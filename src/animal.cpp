#include "globals.h"

Animal::Animal()
    : x(0)
    , y(0)
    , speedX(0)
    , speedY(0)
    , velocity(0)
    , angle(0)
    , radius(assets::kAnimalRadius)
    , texture(nullptr)
{}

Animal::Animal(Texture2D* texture)
    : x(0)
    , y(0)
    , speedX(0)
    , speedY(0)
    , velocity(0)
    , angle(0)
    , radius(assets::kAnimalRadius)
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

void Animal::Update(unsigned char* solid, int w, int h) 
{
    Vector2 v = {speedX, speedY};
    int steps = 7;
    Vector2 dv = { v.x / steps, v.y / steps };

    for (int i = 0; i < steps; ++i) {
        x += dv.x;
        y += dv.y;

        if (CheckCollisionMap(solid, w, h, {x, y}, radius, &seg)) {
            Vector2 p = {x, y};
            Vector2 q = closest_point_seg(p, seg.a, seg.b);
            Vector2 n = normalize(subtract(p, q));
            float push = radius - dist(p, q);
            if (push > 0) { 
                p = add(p, scale(n, push + 0.05f)); 
                x = p.x; 
                y = p.y;
            }
            dv = reflect(dv, n);
            v = reflect(v, n);

            speedX = v.x;
            speedY = v.y;
        }
    }
    if (x < 0 || x > x + w || y < 0 || y < y + h) {
        // Reset position
    }
}

void Animal::Draw() const
{
    if (texture) {
        DrawTexture(*texture, x - radius*2, y - radius*2, {255, 255, 255, 255});
    } else {
        DrawCircle(x, y, radius, PURPLE);
    }
}