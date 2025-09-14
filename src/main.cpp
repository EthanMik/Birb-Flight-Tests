#include "globals.h"

int main() 
{    
    // constexpr int screenWidth = 1440;
    // constexpr int screenHeight = 810;
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "My first RAYLIB program!");
    SetTargetFPS(60);

    Assets::InitAnimalAssets();
    Assets::InitFoodAssets();
    Assets::InitGateAssets();

    Map map{};
    map.Generate({0, 0, 640, 480}, 0);
    
    while (!WindowShouldClose())
    {
        map.Update();
                
        BeginDrawing();
            map.Draw();
                
            for (auto& animal : Animals()) {
                animal.Draw();
            }

        EndDrawing();
    }
    
    CloseWindow();
}