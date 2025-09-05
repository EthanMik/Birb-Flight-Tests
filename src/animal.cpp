#include "globals.h"

Animal::Animal() 
    : x(100)
    , y(100)
    , speedX(0)
    , speedY(0)
    , velocity(10)
    , angle(45)
    , radius(15)
{
    UpdateVelocity(angle);
}

static Vector2 ClosestPointOnSeg(Vector2 p, Vector2 a, Vector2 b) {
    Vector2 ab = subtract(b, a);
    float t = 0.0f, ab2 = dot(ab, ab);
    if (ab2 > 0.0f) {
        t = dot(subtract(p, a), ab) / ab2;
        if (t < 0) { t = 0;} 
        else if (t > 1) { t = 1; } 
    }
    return add(a, scale(ab, t));
}

void Animal::UpdateVelocity(float theta) {
    speedX = velocity * cos(to_rad(theta));
    speedY = velocity * sin(to_rad(theta));
}

void Animal::Update(unsigned char* solid, int w, int h) 
{
    x += speedX;
    y += speedY;

    if (CheckCollisionMap(solid, w, h, {x, y}, radius, &seg)) {
        float a = Reflect(&seg, angle);
        a += Random::get(-10, 10);
        UpdateVelocity(normalize_deg(a));
        angle = a;

        Vector2 p = {x, y};
        Vector2 q = ClosestPointOnSeg(p, seg.a, seg.b);
        Vector2 n = normalize(subtract(p, q));
        float push = radius - dist(p, q);
        if (push > 0) {
            p = add(p, scale(n, push + 1));
            x = p.x; y = p.y;
        }


    }
}

void Animal::Draw() const
{
    DrawCircle(x, y, radius, WHITE);
}