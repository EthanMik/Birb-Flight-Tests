#pragma once

#include "globals.h"

class Animal
{
public:
    Animal();
    void Update(unsigned char* solid, int w, int h);
    void Draw() const;
    
public:
    void UpdateVelocity(float theta);
    segment seg;
    float x;
    float y;
    float velocity;
    float angle;
    float speedX;
    float speedY;
    int radius;
};