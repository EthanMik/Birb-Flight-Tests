#include <raylib.h>
#include "animal.h"
#include "mask.h"

int main() 
{    
    constexpr int screenWidth = 1440;
    constexpr int screenHeight = 810;
    
    Animal a;
    
    InitWindow(screenWidth, screenHeight, "My first RAYLIB program!");
    SetTargetFPS(60);
    
    Image img = LoadImage("map(1).png");
    unsigned char* solid = BuildSolid(&img, 1);

    while (!WindowShouldClose())
    {
        a.Update();
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawCollisionMap(solid, img.width, img.height);
            if (CheckCollisionMap(solid, img.width, img.height, {(float)(a.x), (float)(a.y)}, a.radius)) {
                if (a.speedX > 0 && a.speedY > 0) {
                    a.speedY = -a.speedY;
                }
            }
            // DrawTexture(bg, 0, 0, {255, 255, 255, 255});
            // DrawRectanglePro(rec, {150, 150}, 45, RED);
            a.Draw();
        EndDrawing();
    }
    
    CloseWindow();
}