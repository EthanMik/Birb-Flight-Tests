#include "globals.h"

int main() 
{    
    // constexpr int screenWidth = 1440;
    // constexpr int screenHeight = 810;
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 540;

    std::vector<Animal> animals = {
        Animal{45, RED},
        Animal{50, BLUE},
        Animal{55, YELLOW},
        Animal{60, ORANGE},
        Animal{65, GREEN},
        Animal{70, PURPLE},
        Animal{80, PINK},
        Animal{95, BROWN},
        Animal{100, GRAY}
    };
    
    InitWindow(screenWidth, screenHeight, "My first RAYLIB program!");
    SetTargetFPS(60);
    
    Image img = LoadImage("Map_1.png");
    unsigned char* solid = BuildSolid(&img, 1);

    while (!WindowShouldClose())
    {
        for (auto& animal : animals) {
            animal.Update(solid, img.width, img.height);
        }
        for (size_t i = 0; i < animals.size(); ++i) {
            for (size_t j = 0; j < animals.size(); ++j) {
                ResolveAnimalCollisions(animals[i], animals[j]);
            }
        }
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawCollisionMap(solid, img.width, img.height);
            for (auto& animal : animals) {
               animal.Draw();
            }
        EndDrawing();
    }
    
    CloseWindow();
}