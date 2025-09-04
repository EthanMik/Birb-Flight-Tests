#pragma once

#include "globals.h"

typedef struct { Vector2 a, b; } segment;

inline float to_deg(float rad) {
    return rad * 180 / PI;
}

inline float to_rad(float deg) {
    return deg * PI / 180;
}

inline float normalize_deg(float deg) {
    deg = fmod(deg, 360.0);
    if (deg < 0) {
        deg += 360.0;
    }
    return deg;
}
