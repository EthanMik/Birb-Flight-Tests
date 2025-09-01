#include "animal.h"
#include <raylib.h>
#include <math.h>

Animal::Animal() 
    : x(100)
    , y(400)
    , speedX(8)
    , speedY(8)
    , width(80)
    , height(80)
{
}

static int RandomizeMovement(float speed) {
    // if (speed == 0) {
    //     return 1;
    // }
    return speed;
}

void Animal::Update() 
{
    x += speedX;
    y += speedY;
    
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    

    if ((x + width >= screenWidth) || (x <= 0)) {
        speedY = RandomizeMovement(speedY);
        speedX *= -1;
    }
    
    if ((y + height >= screenHeight) || (y <= 0)) {
        speedX = RandomizeMovement(speedX);
        speedY *= -1;
    }
}

void Animal::Draw() const
{
    DrawRectangle(x, y, width, height, WHITE);
}