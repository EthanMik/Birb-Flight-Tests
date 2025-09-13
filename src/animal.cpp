#include "globals.h"

Animal::Animal()
    : x(0)
    , y(0)
    , speedX(0)
    , speedY(0)
    , velocity(0)
    , angle(0)
    , radius(Assets::kAnimalRadius)
    , texture(nullptr)
{}

Animal::Animal(Texture2D* texture)
    : x(0)
    , y(0)
    , speedX(0)
    , speedY(0)
    , velocity(0)
    , angle(0)
    , radius(Assets::kAnimalRadius)
    , texture(texture)
{}

Animal::Animal(int x, int y, int velocity, int angle, Texture2D* texture) 
    : x(x)
    , y(y)
    , speedX(0)
    , speedY(0)
    , velocity(velocity)
    , angle(angle)
    , radius(10)
    , texture(texture)
{
    UpdateVelocity(angle);
}

Vector2 Animal::GetVelocity() {
    return Vector2{speedX, speedY};
}

void Animal::SetVelocity(Vector2 velocity) {
    speedX = velocity.x;
    speedY = velocity.y;
}

Vector2 Animal::GetPosition() {
    return Vector2{x, y};
}

void Animal::SetPosition(Vector2 position) {
    this->x = position.x;
    this->y = position.y;
}

void Animal::UpdateVelocity(float theta) {
    speedX = velocity * cos(to_rad(theta));
    speedY = -velocity * sin(to_rad(theta));
    angle = theta;
}

void Animal::Draw() const
{
    if (texture) {
        DrawTexture(*texture, x - radius*2, y - radius*2, {255, 255, 255, 255});
    } else {
        DrawCircle(x, y, radius, PURPLE);
    }
}