#pragma once

#include "globals.h"

typedef struct { Vector2 a, b; } segment;


inline float to_deg(float rad) {
    return rad * 180 / PI;
}

inline float to_rad(float deg) {
    return deg * PI / 180;
}

inline float dist(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx*dx + dy*dy);
}

inline float dot(Vector2 a, Vector2 b) {
    return (a.x * b.x + a.y * b.y);
}

inline Vector2 perp(Vector2 a) { 
    return {-a.y, a.x};
}

inline Vector2 scale(Vector2 a, float scale) {
    return Vector2{a.x * scale, a.y * scale};
}

inline Vector2 subtract(Vector2 a, Vector2 b) {
    return Vector2{a.x - b.x, a.y - b.y};
}

inline Vector2 add(Vector2 a, Vector2 b) {
    return Vector2{a.x + b.x, a.y + b.y};
}

inline Vector2 normalize(Vector2 a) {
    float m2 = a.x*a.x + a.y*a.y;
    if (m2 <= 1e-8f) return {0.0f, 0.0f};
    float inv = 1.0f / sqrtf(m2);
    return {a.x * inv, a.y * inv};
}

inline float normalize_deg(float deg) {
    deg = fmod(deg, 360.0);
    if (deg < 0) {
        deg += 360.0;
    }
    return deg;
}

inline Vector2 reflect(Vector2 v, Vector2 n) {
    float vn = v.x * n.x + v.y * n.y;
    return { v.x - 2.0f * vn * n.x, v.y - 2.0f * vn * n.y };
}

inline Vector2 closest_point_seg(Vector2 p, Vector2 a, Vector2 b) {
    Vector2 ab = subtract(b, a);
    float t = 0.0f, ab2 = dot(ab, ab);
    if (ab2 > 0.0f) {
        t = dot(subtract(p, a), ab) / ab2;
        if (t < 0) { t = 0;} 
        else if (t > 1) { t = 1; } 
    }
    return add(a, scale(ab, t));
}