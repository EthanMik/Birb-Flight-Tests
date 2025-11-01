#pragma once

#include "globals.h"

class Animal
{
public:
    Animal();
    Animal(Texture2D* texture);
    Animal(int x, int y, Vector2 velocity, int angle, Texture2D* texture);
    
    void InitalVelocity(float magnitude);
    void InitialPosition(Vector2 position);
    void SetPosition(Vector2 position);
    void SetVelocity(Vector2 velocity);
    Vector2 GetVelocity();
    Vector2 GetPosition();
    bool HasCollision();
    float GetRadius();
    
    void NonColliding(float sec);
    void IncreaseVelocity(float scale, float sec);
    void Kill(float sec);
    void Fling(float scale, float sec);

    void Update();
    void Draw();

private:
    void ResetVelocity();
    void ResetTintColors();
    void UpdateVelocity(float theta);

    Vector2 initialPosition;
    float x;
    float y;
    
    float initalVelocityMag;
    Vector2 velocity;
    float velocityScale;
    float angle;
    
    int radius;
    Texture2D* texture;

    unsigned char alpha{255};
    unsigned char red{255};
    unsigned char green{255};
    unsigned char blue{255};

    bool nonColliding{false};
    bool speedUp{false};
    bool dead{false};
    
    float nonCollidingDuration;
    float speedDuration;
    float killDuration;
    Timer collisionTimer;
    Timer speedTimer;
    Timer killTimer;
};