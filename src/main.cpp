#include "globals.h"

int main() 
{    
    // constexpr int screenWidth = 1440;
    // constexpr int screenHeight = 810;
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "My first RAYLIB program!");
    SetTargetFPS(60);

    Image blackImg = LoadImage("Black.png");
    Image blueImg = LoadImage("Blue.png");
    Image brownImg = LoadImage("Brown.png");
    Image grayImg = LoadImage("Gray.png");
    Image greenImg = LoadImage("Green.png");
    Image pinkImg = LoadImage("Pink.png");
    Image redImg = LoadImage("Red.png");
    Image whiteImg = LoadImage("White.png");
    Image yellowImg = LoadImage("Yellow.png");

    int characterRadius = 10;

    ImageResize(&blackImg, 10*4, 10*4);
    ImageResize(&blueImg, 10*4, 10*4);
    ImageResize(&brownImg, 10*4, 10*4);
    ImageResize(&grayImg, 10*4, 10*4);
    ImageResize(&greenImg, 10*4, 10*4);
    ImageResize(&pinkImg, 10*4, 10*4);
    ImageResize(&redImg, 10*4, 10*4);
    ImageResize(&whiteImg, 10*4, 10*4);
    ImageResize(&yellowImg, 10*4, 10*4);

    Texture2D black = LoadTextureFromImage(blackImg);
    Texture2D blue = LoadTextureFromImage(blueImg);
    Texture2D brown = LoadTextureFromImage(brownImg);
    Texture2D gray = LoadTextureFromImage(grayImg);
    Texture2D green = LoadTextureFromImage(greenImg);
    Texture2D pink = LoadTextureFromImage(pinkImg);
    Texture2D red = LoadTextureFromImage(redImg);
    Texture2D white = LoadTextureFromImage(whiteImg);
    Texture2D yellow = LoadTextureFromImage(yellowImg);

    std::vector<Animal> animals = {
        Animal{45, RED, &black},
        Animal{50, BLUE, &blue},
        Animal{55, YELLOW, &brown},
        Animal{60, ORANGE, &gray},
        Animal{65, GREEN, &green},
        Animal{70, PURPLE, &pink},
        Animal{80, PINK, &red},
        Animal{95, BROWN, &white},
        Animal{100, GRAY, &yellow}
    };
    

    
    Image img = LoadImage("Map_1.png");
    Texture2D texture = LoadTexture("Map_1.png");
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
            // ClearBackground(BLACK);
            DrawTexture(texture, 0, 0, {255, 255, 255, 255});
            // DrawCollisionMap(solid, img.width, img.height);
            for (auto& animal : animals) {
               animal.Draw();
            }
        EndDrawing();
    }
    
    CloseWindow();
}