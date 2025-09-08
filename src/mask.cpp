#include "globals.h"

#include <iostream>
using namespace std;

static void drawLine(Vector2 a, Vector2 b) { 
    DrawLine(a.x, a.y, b.x, b.y, RED);
}

void ResolveAnimalCollisions(Animal& a, Animal& b) {
    if (!CheckAnimalCollision(a, b)) return; 
    
    Vector2 velA = a.GetVelocity();
    Vector2 velB = b.GetVelocity();
    Vector2 posA = a.GetPosition();
    Vector2 posB = b.GetPosition();
    
    Vector2 line = subtract(posB, posA);
    Vector2 normal = normalize(line);

    float approach = dot(subtract(velB, velA), normal);
    if (approach >= 0.0f) return;  

    a.SetVelocity(reflect(velA, normal));
    b.SetVelocity(reflect(velB, normal));
}

bool CheckAnimalCollision(Animal& a, Animal& b) {
    return CheckCollisionCircles(a.GetPosition(), a.radius, b.GetPosition(), b.radius);
} 

unsigned char* BuildSolid(const Image* mask, int alphaThreshold) {
    int w = mask->width;
    int h = mask->height;
    int rez = 7;
    int w_ = w / rez;
    int h_ = h / rez;

    Color* px = LoadImageColors(*mask);
    if (!px) { return nullptr; }

    unsigned char* solid = (unsigned char*)malloc(w_ * h_);
    for (int y_ = 0; y_ < h_; y_++) {
        for (int x_ = 0; x_ < w_; x_++) {
            int x = x_ * rez;
            int y = y_ * rez;
            Color c = px[y * w + x];
            solid[y_ * w_ + x_] = (c.r > alphaThreshold) ? 0 : 1;
            // solid[y_ * w_ + x_] = (c.a > alphaThreshold) ? 0 : 1;
        }
    }
    UnloadImageColors(px);    

    return solid;
}

void DrawCollisionMap(unsigned char* solid, int w, int h) {
    if (!solid) { return; }
    int rez = 7;
    int w_ = w / rez;
    int h_ = h / rez;

    for (int y_ = 0; y_ < h_ - 1; y_++) {
        for (int x_ = 0; x_ < w_ - 1; x_++) {
            int x = x_ * rez;
            int y = y_ * rez;
            Vector2 a = { x, y + rez * 0.5f };
            Vector2 b = { x + rez * 0.5f, y };
            Vector2 c = { x + rez, y + rez * 0.5f };
            Vector2 d = { x + rez * 0.5f, y + rez };

            int c1 = solid[y_ * w_ + x_];
            int c2 = solid[y_ * w_ + (x_ + 1)];
            int c3 = solid[(y_ + 1) * w_ + (x_ + 1)];
            int c4 = solid[(y_ + 1) * w_ + x_];
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

    int rez = 7;
    int w_ = w / rez;
    int h_ = h / rez;
    
    for (int y_ = 0; y_ < h_ - 1; y_++) {
        for (int x_ = 0; x_ < w_ - 1; x_++) {
            int x = x_ * rez;
            int y = y_ * rez;
            Vector2 a = { x, y + rez * 0.5f };
            Vector2 b = { x + rez * 0.5f, y };
            Vector2 c = { x + rez, y + rez * 0.5f };
            Vector2 d = { x + rez * 0.5f, y + rez };

            int c1 = solid[y_ * w_ + x_];
            int c2 = solid[y_ * w_ + (x_ + 1)];
            int c3 = solid[(y_ + 1) * w_ + (x_ + 1)];
            int c4 = solid[(y_ + 1) * w_ + x_];
            int idx = c1 * 8 + c2 * 4 + c3 * 2 + c4 * 1;

            switch (idx) {
                case 0: case 15: break;
                case 1:  if (Collision(center, radius, a, d, seg)) return true; break;
                case 2:  if (Collision(center, radius, c, d, seg)) return true; break;
                case 3:  if (Collision(center, radius, a, c, seg)) return true; break;
                case 4:  if (Collision(center, radius, b, c, seg)) return true; break;
                case 5:  if (Collision(center, radius, a, b, seg) || Collision(center, radius, c, d, seg)) return true; break;
                case 6:  if (Collision(center, radius, b, d, seg)) return true; break;
                case 7:  if (Collision(center, radius, a, b, seg)) return true; break;
                case 8:  if (Collision(center, radius, a, b, seg)) return true; break;
                case 9:  if (Collision(center, radius, b, d, seg)) return true; break;
                case 10: if (Collision(center, radius, a, d, seg) || Collision(center, radius, b, c, seg)) return true; break;
                case 11: if (Collision(center, radius, b, c, seg)) return true; break;
                case 12: if (Collision(center, radius, a, c, seg)) return true; break;
                case 13: if (Collision(center, radius, c, d, seg)) return true; break;
                case 14: if (Collision(center, radius, a, d, seg)) return true; break;
            }
        }
    }
    return false;
}