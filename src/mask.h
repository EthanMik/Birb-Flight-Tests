#pragma once

#include "globals.h"

void ResolveAnimalCollisions(Animal& a, Animal& b);
bool CheckAnimalCollision(Animal& a, Animal& b);
unsigned char* BuildSolid(const Image* mask, int alphaThreshold);
void DrawCollisionMap(unsigned char* solid, int w, int h);
bool CheckCollisionMap(unsigned char* solid, int w, int h, Vector2 center, float radius, segment* seg);