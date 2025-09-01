#pragma once

class Animal
{
public:
    Animal();
    void Update();
    void Draw() const;

private:
    int x;
    int y;
    int speedX;
    int speedY;
    int width;
    int height;
};