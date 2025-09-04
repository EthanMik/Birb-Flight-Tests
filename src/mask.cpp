#include "globals.h"

#include <iostream>
using namespace std;

static void drawLine(Vector2 a, Vector2 b) { 
    DrawLine(a.x, a.y, b.x, b.y, RED);
}

float Reflect(segment* seg, float angle) {
    cout << "Start A:" << angle << endl;
    cout << "<" << seg->a.x << " " << seg->a.y << ">" << "<" << seg->b.x << " " << seg->b.y << ">" << endl;

    float tangent = atan2(seg->b.y - seg->a.y, seg->b.x - seg->a.x);
    float tau = 2 * PI;
    float out = fmod((2 * tangent - to_rad(angle)), tau);

    if (out < 0.0f) { out += tau; }
    cout << to_deg(out) << endl;
    
    return to_deg(out);
}

unsigned char* BuildSolid(const Image* mask, int alphaThreshold) {
    int w = mask->width;
    int h = mask->height;

    Color* px = LoadImageColors(*mask);
    if (!px) { return nullptr; }

    unsigned char* solid = (unsigned char*)malloc(w * h);
    for (int y = 0; y < h; y++) 
        for (int x = 0; x < w; x++) {
        Color c = px[y * w + x];
        solid[y * w + x] = (c.a > alphaThreshold) ? 1 : 0;
    }
    UnloadImageColors(px);    

    return solid;
}

void DrawCollisionMap(unsigned char* solid, int w, int h) {
    if (!solid) { return; }

    for (int y = 0; y < h - 1; y++) {
        for (int x = 0; x < w - 1; x++) {
            Vector2 a = { x, y + 0.5f };
            Vector2 b = { x + 0.5f, y };
            Vector2 c = { x + 1.0f, y + 0.5f };
            Vector2 d = { x + 0.5f, y + 1.0f };

            int c1 = solid[y * w + x];
            int c2 = solid[y * w + (x + 1)];
            int c3 = solid[(y + 1) * w + (x + 1)];
            int c4 = solid[(y + 1) * w + x];
            int idx = c1 * 8 + c2 * 4 + c3 * 2 + c4 * 1;

            switch (idx) {
                case 0:  break;
                case 1:  drawLine(a, d); break;
                case 2:  drawLine(c, d); break; 
                case 3:  drawLine(a, c); break;                
                case 4:  drawLine(b, c); break; 
                case 5:  drawLine(a, b); drawLine(c, d); break; 
                case 6:  drawLine(b, d); break;
                case 7:  drawLine(a, b); break;
                case 8:  drawLine(a, b); break;
                case 9:  drawLine(b, d); break;
                case 10: drawLine(a, d); drawLine(b, c); break;
                case 11: drawLine(b, c); break;
                case 12: drawLine(a, c); break;
                case 13: drawLine(c, d); break;
                case 14: drawLine(a, d); break;
                case 15: break;
            }
        }
    }
}

static bool Collision(Vector2 center, float radius, Vector2 a, Vector2 b, segment* seg) {
    if (CheckCollisionCircleLine(center, radius, a, b)) {
        if (seg) {
            seg->a = a;
            seg->b = b;
        }
        return true;
    }
    return false;
}

bool CheckCollisionMap(unsigned char* solid, int w, int h, Vector2 center, float radius, segment* seg) {
    if (!solid) { return false; }

    for (int y = 0; y < h - 1; y++) {
        for (int x = 0; x < w - 1; x++) {
            Vector2 a = { x, y + 0.5f };
            Vector2 b = { x + 0.5f, y };
            Vector2 c = { x + 1.0f, y + 0.5f };
            Vector2 d = { x + 0.5f, y + 1.0f };

            int c1 = solid[y * w + x];
            int c2 = solid[y * w + (x + 1)];
            int c3 = solid[(y + 1) * w + (x + 1)];
            int c4 = solid[(y + 1) * w + x];
            int idx = c1 * 8 + c2 * 4 + c3 * 2 + c4 * 1;
            
            switch (idx) {
                case 0: case 15: break;
                case 1:  if (Collision(center, radius, a, d, seg)) return true; break;
                case 2:  if (Collision(center, radius, c, d, seg)) return true; break;
                case 3:  if (Collision(center, radius, a, c, seg)) return true; break;
                case 4:  if (Collision(center, radius, b, c, seg)) return true; break;
                case 5:  if (Collision(center, radius, a, b, seg) ||
                             Collision(center, radius, c, d, seg)) return true; break;
                case 6:  if (Collision(center, radius, b, d, seg)) return true; break;
                case 7:  if (Collision(center, radius, a, b, seg)) return true; break;
                case 8:  if (Collision(center, radius, a, b, seg)) return true; break;
                case 9:  if (Collision(center, radius, b, d, seg)) return true; break;
                case 10: if (Collision(center, radius, a, d, seg) ||
                             Collision(center, radius, b, c, seg)) return true; break;
                case 11: if (Collision(center, radius, b, c, seg)) return true; break;
                case 12: if (Collision(center, radius, a, c, seg)) return true; break;
                case 13: if (Collision(center, radius, c, d, seg)) return true; break;
                case 14: if (Collision(center, radius, a, d, seg)) return true; break;
            }
        }
    }
    return false;
}