#pragma once

#include "globals.h"

unsigned char* BuildSolid(const Image* mask, int alphaThreshold);
float Reflect(segment* seg, float angle);
void DrawCollisionMap(unsigned char* solid, int w, int h);
bool CheckCollisionMap(unsigned char* solid, int w, int h, Vector2 center, float radius, segment* seg);