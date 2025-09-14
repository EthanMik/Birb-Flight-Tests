#pragma once

#include "globals.h"

class Animal
{
public:
    Animal();
    Animal(Texture2D* texture);
    Animal(int x, int y, int velocity, int angle, Texture2D* texture);
    
    void Draw() const;
    
    void SetPosition(Vector2 position);
    void SetVelocity(Vector2 velocity);
    Vector2 GetVelocity();
    Vector2 GetPosition();

    float Radius();

private:
    void UpdateVelocity(float theta);

    float x;
    float y;

    float angle;
    float velocity;
    float speedX;
    float speedY;

    int radius;
    Texture2D* texture;
};