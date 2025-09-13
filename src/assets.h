#pragma once

#include "globals.h"

namespace Assets
{
    inline constexpr std::array<const char*, 1> kFoodFiles = {
        "assets/Seeds.png"
    };    
}

// Gate Assets
namespace Assets
{
    inline constexpr std::array<const char*, 1> kGateFiles = {
        "assets/Gate.jpg"
    };    
}

// Map Assets
namespace Assets
{
    inline constexpr std::array<const char*, 1> kMapFiles = {
        "assets/Maps/Map_1.png"
    };
}

// Animal Assets
namespace Assets {
    inline constexpr int kAnimalRadius = 10;
    inline constexpr int kAnimalScale = 4;

    inline constexpr std::array<const char*, 9> kAnimalFiles = {
        "assets/Birbs/Black.png",
        "assets/Birbs/Blue.png",
        "assets/Birbs/Brown.png",
        "assets/Birbs/Gray.png",
        "assets/Birbs/Green.png",
        "assets/Birbs/Pink.png",
        "assets/Birbs/Red.png",
        "assets/Birbs/White.png",
        "assets/Birbs/Yellow.png"
    };

    inline std::array<Texture2D, kAnimalFiles.size()> g_animalTextures{};
    inline std::array<Animal, kAnimalFiles.size()> g_animals;

    inline void InitAnimalAssets() {
        for (size_t i = 0; i < kAnimalFiles.size(); ++i) {
            const char* path = kAnimalFiles[i];
            
            Image img = LoadImage(path);

            if (img.data == nullptr) {
                TraceLog(LOG_ERROR, "Failed to load image: %s", path);
                g_animals[i] = Animal{};
                continue;
            }

            int animalSize = kAnimalRadius * kAnimalScale;
            ImageResize(&img, animalSize, animalSize);

            g_animalTextures[i] = LoadTextureFromImage(img);
            g_animals[i] = Animal{&g_animalTextures[i]};
        }        
    }

};

inline std::array<Animal, Assets::kAnimalFiles.size()>& Animals() {
    return Assets::g_animals;
}

