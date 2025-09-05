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
    return sqrt(pow((b.x - a.x), 2) + pow((b.y - b.y), 2));
}

inline float dot(Vector2 a, Vector2 b) {
    return (a.x * b.x + a.y * b.y);
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
    float mag = sqrt(a.x * a.x + a.y * a.y);
    return Vector2{a.x / mag, a.y / mag}; 
}

inline float normalize_deg(float deg) {
    deg = fmod(deg, 360.0);
    if (deg < 0) {
        deg += 360.0;
    }
    return deg;
}
