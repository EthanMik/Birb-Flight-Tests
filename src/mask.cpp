#include "raylib.h"
#include <stdlib.h>

typedef struct {int x, y; } I2;  
typedef struct { Vector2 a, b; } segment;

static void drawLine(Vector2 a, Vector2 b) {
    DrawLine(a.x, a.y, b.x, b.y, RED);
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

bool CheckCollisionMap(unsigned char* solid, int w, int h, Vector2 center, float radius) {
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
                case 1:  if (CheckCollisionCircleLine(center, radius, a, d)) return true; break;
                case 2:  if (CheckCollisionCircleLine(center, radius, c, d)) return true; break;
                case 3:  if (CheckCollisionCircleLine(center, radius, a, c)) return true; break;
                case 4:  if (CheckCollisionCircleLine(center, radius, b, c)) return true; break;
                case 5:  if (CheckCollisionCircleLine(center, radius, a, b) ||
                             CheckCollisionCircleLine(center, radius, c, d)) return true; break;
                case 6:  if (CheckCollisionCircleLine(center, radius, b, d)) return true; break;
                case 7:  if (CheckCollisionCircleLine(center, radius, a, b)) return true; break;
                case 8:  if (CheckCollisionCircleLine(center, radius, a, b)) return true; break;
                case 9:  if (CheckCollisionCircleLine(center, radius, b, d)) return true; break;
                case 10: if (CheckCollisionCircleLine(center, radius, a, d) ||
                             CheckCollisionCircleLine(center, radius, b, c)) return true; break;
                case 11: if (CheckCollisionCircleLine(center, radius, b, c)) return true; break;
                case 12: if (CheckCollisionCircleLine(center, radius, a, c)) return true; break;
                case 13: if (CheckCollisionCircleLine(center, radius, c, d)) return true; break;
                case 14: if (CheckCollisionCircleLine(center, radius, a, d)) return true; break;
            }
        }
    }
    return false;
}