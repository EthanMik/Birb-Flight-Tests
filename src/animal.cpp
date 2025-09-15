#include "globals.h"

Animal::Animal()
    : x(0)
    , y(0)
    , velocity({0, 0})
    , angle(0)
    , radius(Assets::kAnimalRadius)
    , texture(nullptr)
{}

Animal::Animal(Texture2D* texture)
    : x(0)
    , y(0)
    , velocity({0, 0})
    , angle(0)
    , radius(Assets::kAnimalRadius)
    , texture(texture)
{}

Animal::Animal(int x, int y, Vector2 velocity, int angle, Texture2D* texture) 
    : x(x)
    , y(y)
    , velocity(velocity)
    , angle(angle)
    , radius(Assets::kAnimalRadius)
    , texture(texture)
{
    UpdateVelocity(angle);
}

Vector2 Animal::GetVelocity() {
    return velocity;
}

void Animal::SetVelocity(Vector2 velocity) {
    this->velocity = velocity;
}

Vector2 Animal::GetPosition() {
    return Vector2{x, y};
}

void Animal::SetPosition(Vector2 position) {
    this->x = position.x;
    this->y = position.y;
}

float Animal::GetRadius() { return radius; }

bool Animal::HasCollision() { return !nonColliding; }

void Animal::NonColliding(float time) {
    nonColliding = true;
    nonCollidingDuration = time;
    alpha /= 2;
    timer.reset();
}

void Animal::IncreaseVelocity(float scale) {
    Vector2 newVelocity {velocity.x * scale, velocity.y * scale};
    if (abs(newVelocity.x) > maxSpeed.x 
        || abs(newVelocity.y) > maxSpeed.y) return;
    
    velocity = newVelocity;
}


void Animal::Update() {
    if (timer.elapsed() >= nonCollidingDuration) {
        alpha = 255;
        nonColliding = false;
    }
}

void Animal::Draw() {
    if (texture) {
        DrawTexture(*texture, x - radius*2, y - radius*2, {255, 255, 255, alpha});
    } else {
        DrawCircle(x, y, radius, PURPLE);
    }
}

void Animal::UpdateVelocity(float theta) {
    velocity.x *= cos(to_rad(theta));
    velocity.y *= -sin(to_rad(theta));
    angle = theta;
}