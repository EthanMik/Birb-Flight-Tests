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

void Animal::InitalVelocity(float magnitude) {
    this->initalVelocityMag = magnitude;
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

void Animal::InitialPosition(Vector2 position) {
    initialPosition = position;
    this->x = position.x;
    this->y = position.y;
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
    alpha = 120;
    collisionTimer.reset();
}

void Animal::ResetVelocity() {
    Vector2 newVelocity {velocity.x / velocityScale, velocity.y / velocityScale};
    velocity = newVelocity;    
}

void Animal::ResetTintColors() {
    red = 255;
    blue = 255;
    green = 255;
}

void Animal::IncreaseVelocity(float scale, float time) {
    speedDuration = time;
    velocityScale = scale;
    speedTimer.reset();
    if (speedUp) return;
    speedUp = true;
    
    Vector2 newVelocity {velocity.x * scale, velocity.y * scale};
    velocity = newVelocity;
}

void Animal::Update() {
    if (collisionTimer.elapsed() >= nonCollidingDuration && nonColliding) {
        alpha = 255;
        nonColliding = false;
    }

    if (speedTimer.elapsed() >= speedDuration && speedUp) {
        ResetVelocity();
        speedUp = false;
    }

    if (killTimer.elapsed() >= killDuration && dead) {
        this->SetPosition(initialPosition);
        this->SetVelocity(components(Random::get(0, 360), initalVelocityMag));
        ResetTintColors();
        dead = false;
    }

    if (dead) {
        float factor = 255 / killDuration / 60;
        green -= factor;
        blue -= factor;
    }
}

void Animal::Fling(float scale, float time) {
    Vector2 newVelocity {-velocity.x, -velocity.y};
    velocity = newVelocity;        
    IncreaseVelocity(scale, time);
}

void Animal::Kill(float time) {
    dead = true;
    killTimer.reset();
    killDuration = time;
}

void Animal::Draw() {
    const float maxGlowRadius = radius * 1.7;
    const float minGlowRadius = radius * 1.1;
    static float glowRadius = maxGlowRadius;

    if (texture) {
        if (speedUp) {
            DrawCircle(x, y, glowRadius, {87, 185, 255, 150});
            glowRadius *= .95;
            if (glowRadius < minGlowRadius) glowRadius = maxGlowRadius;
        }
        DrawTexture(*texture, x - radius*2, y - radius*2, {red, green, blue, alpha});

    } else {
        DrawCircle(x, y, radius, PURPLE);
    }
}

void Animal::UpdateVelocity(float theta) {
    velocity.x *= cos(to_rad(theta));
    velocity.y *= -sin(to_rad(theta));
    angle = theta;
}