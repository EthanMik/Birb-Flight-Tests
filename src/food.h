#pragma once

#include "globals.h"

class Food
{
public:
    Food(float x, float y, Texture2D* texture);

    void Draw();

private:
    float x;
    float y; 
    Texture2D* texture;
};