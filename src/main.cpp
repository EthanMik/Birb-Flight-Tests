#include <raylib.h>
#include "animal.h"
#include "mask.h"

int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    constexpr int screenWidth = 1440;
    constexpr int screenHeight = 810;
    
    Animal animal;
    
    InitWindow(screenWidth, screenHeight, "My first RAYLIB program!");
    SetTargetFPS(60);
    
    Texture2D bg = LoadTexture("preview.jpg");
    Image img = LoadImage("map(1).png");
    Rectangle rec{500, 500, 100, 100};

    while (!WindowShouldClose())
    {
        animal.Update();
        
        BeginDrawing();
            BuildCollisionMap(img, 30, 1);
            // DrawTexture(bg, 0, 0, {255, 255, 255, 255});
            // DrawRectanglePro(rec, {150, 150}, 45, RED);
            // // ClearBackground(darkGreen);
            // animal.Draw();
        EndDrawing();
    }
    
    CloseWindow();
}