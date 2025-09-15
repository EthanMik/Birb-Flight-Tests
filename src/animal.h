#pragma once

#include "globals.h"

class Animal
{
public:
    Animal();
    Animal(Texture2D* texture);
    Animal(int x, int y, Vector2 velocity, int angle, Texture2D* texture);
    
    void SetPosition(Vector2 position);
    void SetVelocity(Vector2 velocity);
    Vector2 GetVelocity();
    Vector2 GetPosition();
    bool HasCollision();
    float GetRadius();
    
    void NonColliding(float sec);
    void IncreaseVelocity(float scale);
    void Kill();
    void Fling();

    void Update();
    void Draw();

private:
    void UpdateVelocity(float theta);

    float x;
    float y;

    Vector2 maxSpeed{2.5, 2.5};
    Vector2 velocity;
    float angle;
    
    int radius;
    Texture2D* texture;

    unsigned char alpha{255};
    bool nonColliding{false};
    float nonCollidingDuration;
    Timer timer;
};