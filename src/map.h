#pragma once

#include "globals.h"

class Map 
{
public:
    Map();
    ~Map();

    void Generate(Rectangle mapRegion, int mapIndex = -1);
    void Update();
    void Draw();

private:
    void PlaceFood(float x, float y, size_t count);
    void PlaceGate(float x, float y, size_t count);
    void PlaceAnimal(float x, float y, size_t count);
    void BuildWalls(const Image* mask, int resolution);
    bool CheckWallCollisions(Vector2 animalPosition, segment* seg);

    void ResolveAnimalCollisions();
    void ResolveWallCollisions();
    bool ResolveFoodCollisions();

    void test_DrawWalls();

    std::vector<Food> foods;
    std::vector<Gate> gates;

    unsigned char* map;
    Rectangle mapRegion;
    Texture2D mapTexture;
    int mapResolution;
};