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

    Image seedsImg = LoadImage("assets/Seeds.png");
    ImageResize(&seedsImg, 30, 30);
    Texture2D seeds = LoadTextureFromImage(seedsImg);
    
    Image img = LoadImage("assets/Maps/Map_1.png");
    Texture2D texture = LoadTexture("assets/Maps/Map_1.png");

    unsigned char* solid = BuildSolid(&img, 1);
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