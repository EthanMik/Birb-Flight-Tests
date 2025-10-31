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
    Assets::InitOverlayAssets();

    Map map{};
    Overlay overlay{};

    map.Generate({0, 60, 640, 480}, 0);
    
    while (!WindowShouldClose())
    {
        map.Update();
                
        BeginDrawing();
            map.Draw();
                
            for (auto& animal : Animals()) {
                animal.Draw();
            }

            overlay.Draw();

        EndDrawing();
    }
    
    CloseWindow();
}