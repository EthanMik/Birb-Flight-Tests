#pragma once

#include "globals.h"

class Food
{
public:
    Food(float x, float y, Texture2D* texture);

    void Draw();
    Rectangle Region();

private:
    float x;
    float y; 
    Texture2D* texture;
};