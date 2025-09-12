#pragma once

#include "globals.h"

class Map 
{
public:
    Map();
    ~Map();

    void Generate(Rectangle mapRegion);
    void Update();
    void Draw();

private:
    void PlaceAnimal(float x, float y, size_t count);
    void BuildWalls(const Image* mask, int resolution);
    void CheckWallCollisions();
    void ResolveAnimalCollisions();
    void ResolveWallCollisions();

    void test_DrawWalls();
    
    Rectangle mapRegion;
    Texture2D mapTexture;
    unsigned char* map;
};