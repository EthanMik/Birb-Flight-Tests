#pragma once

#include "globals.h"

class Animal
{
public:
    Animal(int angle, Color color, Texture2D* texture);
    void Update(unsigned char* solid, int w, int h);
    void Draw() const;
    
    void SetVelocity(Vector2 velocity);
    Vector2 GetVelocity();
    Vector2 GetPosition();
public:
    void UpdateVelocity(float theta);
    segment seg;
    float x;
    float y;
    int randomness;
    float velocity;
    float angle;
    float speedX;
    float speedY;
    int radius;
    Color color;
    Timer timer;
    Texture2D* texture;
};