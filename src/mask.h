#pragma once

#include <raylib.h>

unsigned char* BuildSolid(const Image* mask, int alphaThreshold);
void DrawCollisionMap(unsigned char* solid, int w, int h);
bool CheckCollisionMap(unsigned char* solid, int w, int h, Vector2 center, float radius);