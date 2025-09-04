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
    }

    // if (CheckCollisionMap(solid, w, h, {x, y}, radius, &seg)) {
    //     // depenetrate
    //     Vector2 p = {x, y};
    //     Vector2 q = ClosestPointOnSeg(p, seg.a, seg.b);
    //     Vector2 n = Vector2Normalize(Vector2Subtract(p, q));
    //     float push = radius - Vector2Distance(p, q);
    //     if (push > 0) {
    //         p = Vector2Add(p, Vector2Scale(n, push + 0.5f)); // small epsilon
    //         x = p.x; y = p.y;
    //     }

    //     float a = Reflect(&seg, angle);
    //     angle = a;
    //     UpdateVelocity(a);

}

void Animal::Draw() const
{
    DrawCircle(x, y, radius, WHITE);
}