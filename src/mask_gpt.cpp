// mask_polys.h
// Build polygons from an RGBA mask image (alpha > threshold = solid).
// Also: general polygon collision helpers (poly-vs-circle/rect/poly).
#ifndef MASK_POLYS_H
#define MASK_POLYS_H

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct { Vector2 *v; int count; } Poly;
typedef struct { Poly *items; int count; } PolySet;

static float distance_point_segment(Vector2 p, Vector2 a, Vector2 b) {
    Vector2 ab = Vector2Subtract(b, a);
    float t = Vector2Dot(Vector2Subtract(p, a), ab) / Vector2LengthSqr(ab);
    t = fmaxf(0.0f, fminf(1.0f, t));
    Vector2 q = Vector2Add(a, Vector2Scale(ab, t));
    return Vector2Distance(p, q);
}

static bool segments_intersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
    // Proper & collinear intersection
    Vector2 r = Vector2Subtract(b, a);
    Vector2 s = Vector2Subtract(d, c);
    float rxs = r.x*s.y - r.y*s.x;
    float qpxr = (c.x - a.x)*r.y - (c.y - a.y)*r.x;
    if (fabsf(rxs) < 1e-7f && fabsf(qpxr) < 1e-7f) {
        // collinear: check overlap
        float t0 = Vector2Dot(Vector2Subtract(c,a), r) / Vector2LengthSqr(r);
        float t1 = t0 + Vector2Dot(s, r) / Vector2LengthSqr(r);
        if (t0 > t1) { float tmp=t0; t0=t1; t1=tmp; }
        return !(t1 < 0.0f || t0 > 1.0f);
    }
    if (fabsf(rxs) < 1e-7f) return false; // parallel non-intersecting
    float t = ((c.x - a.x)*s.y - (c.y - a.y)*s.x) / rxs;
    float u = ((c.x - a.x)*r.y - (c.y - a.y)*r.x) / rxs;
    return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}

static bool point_in_polygon(Vector2 p, const Vector2 *poly, int n) {
    // ray-cast (even-odd)
    bool inside = false;
    for (int i=0, j=n-1; i<n; j=i++) {
        Vector2 a = poly[j], b = poly[i];
        bool cond = ((a.y > p.y) != (b.y > p.y)) &&
                    (p.x < (b.x - a.x)*(p.y - a.y)/(b.y - a.y + 1e-12f) + a.x);
        if (cond) inside = !inside;
    }
    return inside;
}

static bool poly_vs_poly(const Vector2 *A, int nA, const Vector2 *B, int nB) {
    // general (concave ok): edge intersections OR containment
    for (int i=0;i<nA;i++) {
        Vector2 a0=A[i], a1=A[(i+1)%nA];
        for (int j=0;j<nB;j++) {
            Vector2 b0=B[j], b1=B[(j+1)%nB];
            if (segments_intersect(a0,a1,b0,b1)) return true;
        }
    }
    if (point_in_polygon(A[0], B, nB)) return true;
    if (point_in_polygon(B[0], A, nA)) return true;
    return false;
}

static bool poly_vs_circle(const Vector2 *P, int n, Vector2 c, float r) {
    if (point_in_polygon(c, P, n)) return true;
    for (int i=0;i<n;i++) {
        Vector2 a=P[i], b=P[(i+1)%n];
        if (distance_point_segment(c, a, b) <= r) return true;
    }
    return false;
}

static bool poly_vs_rec(const Vector2 *P, int n, Rectangle rec) {
    Vector2 R[4] = {
        {rec.x, rec.y},
        {rec.x+rec.width, rec.y},
        {rec.x+rec.width, rec.y+rec.height},
        {rec.x, rec.y+rec.height}
    };
    return poly_vs_poly(P, n, R, 4);
}

// ---------- RDP simplification ----------
static float perpendicular_distance(Vector2 p, Vector2 a, Vector2 b) {
    return distance_point_segment(p, a, b);
}

static void rdp_rec(const Vector2 *pts, int start, int end, float eps, bool *keep) {
    if (end <= start+1) return;
    float dmax = 0.0f; int idx = start;
    for (int i=start+1;i<end;i++) {
        float d = perpendicular_distance(pts[i], pts[start], pts[end]);
        if (d > dmax) { dmax = d; idx = i; }
    }
    if (dmax > eps) {
        keep[idx] = true;
        rdp_rec(pts, start, idx, eps, keep);
        rdp_rec(pts, idx, end, eps, keep);
    }
}

static Poly simplify_polygon(Poly in, float eps) {
    if (in.count < 3) return in;
    // ensure closed loop for RDP across wrap-around
    // do RDP on open chain (0..n-1) then re-close
    bool *keep = (bool*)calloc(in.count, sizeof(bool));
    keep[0] = keep[in.count-1] = true;
    rdp_rec(in.v, 0, in.count-1, eps, keep);
    // compact
    Vector2 *outv = (Vector2*)malloc(sizeof(Vector2)*in.count);
    int m=0;
    for (int i=0;i<in.count;i++) if (keep[i]) outv[m++] = in.v[i];
    free(keep);
    Poly out = { outv, m };
    return out;
}

// ---------- Marching Squares to polygons ----------
typedef struct { int x,y; } I2;
typedef struct { I2 a,b; } Seg;

static inline I2 I2mk(int x,int y){ I2 p={x,y}; return p; }
static inline bool I2eq(I2 a, I2 b){ return a.x==b.x && a.y==b.y; }

typedef struct { Seg *data; int n, cap; } SegList;
static void seg_push(SegList *s, Seg e){ if(s->n==s->cap){ s->cap=(s->cap? s->cap*2:64); s->data=(Seg*)realloc(s->data,sizeof(Seg)*s->cap);} s->data[s->n++]=e; }

static void add_case_segments(int x, int y, unsigned idx, SegList *out) {
    // midpoints in doubled grid coordinates
    I2 L = I2mk(2*x,   2*y+1);
    I2 T = I2mk(2*x+1, 2*y);
    I2 R = I2mk(2*x+2, 2*y+1);
    I2 B = I2mk(2*x+1, 2*y+2);

    switch(idx){
        case 0: case 15: break;
        case 1:  seg_push(out, (Seg){L,T}); break;            // TL
        case 2:  seg_push(out, (Seg){T,R}); break;            // TR
        case 3:  seg_push(out, (Seg){L,R}); break;            // TL+TR
        case 4:  seg_push(out, (Seg){R,B}); break;            // BR
        case 5:  seg_push(out, (Seg){L,T}); seg_push(out,(Seg){R,B}); break; // TL+BR
        case 6:  seg_push(out, (Seg){T,B}); break;            // TR+BR
        case 7:  seg_push(out, (Seg){L,B}); break;            // TL+TR+BR
        case 8:  seg_push(out, (Seg){B,L}); break;            // BL
        case 9:  seg_push(out, (Seg){T,B}); break;            // TL+BL
        case 10: seg_push(out, (Seg){T,R}); seg_push(out,(Seg){B,L}); break; // TR+BL
        case 11: seg_push(out, (Seg){B,R}); break;            // TL+TR+BL
        case 12: seg_push(out, (Seg){R,L}); break;            // BR+BL
        case 13: seg_push(out, (Seg){T,R}); break;            // TL+BR+BL
        case 14: seg_push(out, (Seg){L,T}); break;            // TR+BR+BL
    }
}

static PolySet BuildPolygonsFromAlpha(Image mask, unsigned char alphaThreshold,
                                      float pixelsToWorld, float simplifyEpsPixels)
{
    int W = mask.width, H = mask.height;
    Color *px = LoadImageColors(mask);
    if (!px) { PolySet empty = {0}; return empty; }

    // 1) binary field (samples at pixels)
    unsigned char *solid = (unsigned char*)malloc(W*H);
    for (int y=0;y<H;y++) for (int x=0;x<W;x++) {
        Color c = px[y*W+x];
        solid[y*W+x] = (c.a > alphaThreshold) ? 1 : 0;
    }
    UnloadImageColors(px);

    // 2) marching squares => segments
    SegList segs = {0};
    for (int y=0;y<H-1;y++) for (int x=0;x<W-1;x++) {
        // corners: TL, TR, BR, BL -> bits 1,2,4,8
        int tl = solid[y*W + x];
        int tr = solid[y*W + (x+1)];
        int br = solid[(y+1)*W + (x+1)];
        int bl = solid[(y+1)*W + x];
        unsigned idx = (tl?1:0) | (tr?2:0) | (br?4:0) | (bl?8:0);
        add_case_segments(x, y, idx, &segs);
    }
    free(solid);

    // 3) stitch segments into closed loops (O(n^2) is fine for level masks)
    //    Points live on doubled grid; convert to world coords at the end.
    bool *used = (bool*)calloc(segs.n, sizeof(bool));
    Poly *polys = NULL; int polyCap=0, polyCount=0;

    for (int i=0;i<segs.n;i++) if (!used[i]) {
        // start a chain
        I2 start = segs.data[i].a;
        I2 currEnd = segs.data[i].b;
        used[i] = true;

        // dynamic list of points
        int cap=128, n=0;
        Vector2 *pts = (Vector2*)malloc(sizeof(Vector2)*cap);
        // push start and end
        pts[n++] = (Vector2){ start.x*0.5f * pixelsToWorld, start.y*0.5f * pixelsToWorld };
        pts[n++] = (Vector2){ currEnd.x*0.5f * pixelsToWorld, currEnd.y*0.5f * pixelsToWorld };

        // keep walking until we close the loop
        bool closed = I2eq(currEnd, start);
        while (!closed) {
            bool advanced = false;
            for (int j=0;j<segs.n;j++) if (!used[j]) {
                if (I2eq(segs.data[j].a, currEnd)) {
                    currEnd = segs.data[j].b;
                    used[j]=true; advanced=true;
                } else if (I2eq(segs.data[j].b, currEnd)) {
                    currEnd = segs.data[j].a;
                    used[j]=true; advanced=true;
                }
                if (advanced) {
                    if (n==cap){ cap*=2; pts=(Vector2*)realloc(pts,sizeof(Vector2)*cap); }
                    pts[n++] = (Vector2){ currEnd.x*0.5f * pixelsToWorld, currEnd.y*0.5f * pixelsToWorld };
                    break;
                }
            }
            if (!advanced) break; // open chain (shouldn't happen for closed solids)
            closed = I2eq(currEnd, start);
        }

        // de-duplicate last = first
        if (n>=2 && Vector2DistanceSqr(pts[0], pts[n-1]) < 1e-10f) n--;

        // optional simplify in pixel units (scaled to world)
        Poly raw = { pts, n };
        Poly simp = (simplifyEpsPixels > 0.0f) ? simplify_polygon(raw, simplifyEpsPixels * pixelsToWorld) : raw;
        if (simp.v != pts) free(pts); // raw freed if replaced

        if (polyCount==polyCap){ polyCap = (polyCap? polyCap*2:16); polys=(Poly*)realloc(polys,sizeof(Poly)*polyCap); }
        polys[polyCount++] = simp;
    }

    free(used);
    free(segs.data);

    PolySet out = { polys, polyCount };
    return out;
}

static void FreePolySet(PolySet *ps) {
    if (!ps || !ps->items) return;
    for (int i=0;i<ps->count;i++) free(ps->items[i].v);
    free(ps->items); ps->items=NULL; ps->count=0;
}

#endif // MASK_POLYS_H

// main.c
#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "mask_polys.h"   // from above

int main(void) {
    InitWindow(1280, 720, "raylib image->polygon collisions");
    SetTargetFPS(60);

    // Load your mask (same size as background)
    Image mask = LoadImage("level_mask.png"); // transparent outside, opaque on solids
    if (mask.data == NULL) { TraceLog(LOG_ERROR, "Failed to load mask"); return 1; }

    // Pixels->world scale: 1.0f means 1 pixel == 1 world unit
    float pixelsToWorld = 1.0f;
    PolySet level = BuildPolygonsFromAlpha(mask, 16 /*alpha threshold*/,
                                           pixelsToWorld, 2.0f /*simplify epsilon in pixels*/);
    UnloadImage(mask);

    // Optional: background art (for rendering)
    Texture bg = LoadTexture("level_art.png");

    Vector2 player = { 200, 200 };
    float   radius = 14.0f;

    while (!WindowShouldClose()) {
        // --- update ---
        float spd = 140.0f * GetFrameTime();
        if (IsKeyDown(KEY_LEFT))  player.x -= spd;
        if (IsKeyDown(KEY_RIGHT)) player.x += spd;
        if (IsKeyDown(KEY_UP))    player.y -= spd;
        if (IsKeyDown(KEY_DOWN))  player.y += spd;

        // --- detect collisions (circle vs polygons) ---
        bool hit = false;
        int hitIndex = -1;
        for (int i=0;i<level.count;i++) {
            if (poly_vs_circle(level.items[i].v, level.items[i].count, player, radius)) {
                hit = true; hitIndex = i; break;
            }
        }

        // --- draw ---
        BeginDrawing();
        ClearBackground((Color){20,20,24,255});
        if (bg.id) DrawTexture(bg, 0, 0, WHITE);

        // draw polygons
        for (int i=0;i<level.count;i++) {
            Color c = (i==hitIndex)? RED : (Color){80, 255, 120, 180};
            Poly P = level.items[i];
            for (int j=0;j<P.count;j++) {
                Vector2 a = P.v[j];
                Vector2 b = P.v[(j+1)%P.count];
                DrawLineEx(a, b, 2.0f, c);
            }
        }

        // player
        DrawCircleV(player, radius, (hit? ORANGE: SKYBLUE));
        DrawCircleLines((int)player.x, (int)player.y, radius, BLACK);

        DrawText("Arrows to move. Collision turns the circle orange.", 18, 18, 18, RAYWHITE);
        EndDrawing();
    }

    if (bg.id) UnloadTexture(bg);
    FreePolySet(&level);
    CloseWindow();
    return 0;
}
