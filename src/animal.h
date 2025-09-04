#pragma once

class Animal
{
public:
    Animal();
    void Update();
    void Draw() const;
    
public:
    int x;
    int y;
    int speedX;
    int speedY;
    int radius;
};