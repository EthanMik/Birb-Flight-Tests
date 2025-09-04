#pragma once

#include "globals.h"

typedef struct { Vector2 a, b; } segment;

inline float to_deg(float rad) {
    return rad * 180 / PI;
}

inline float to_rad(float deg) {
    return deg * PI / 180;
}
