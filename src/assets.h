#pragma once

#include "globals.h"

// Food Assets
namespace Assets
{
    inline constexpr std::array<const char*, 1> kFoodFiles = {
        "assets/Seeds.png"
    };    

    inline std::array<Texture2D, kFoodFiles.size()> g_FoodTextures;

    inline constexpr int kFoodWidth = 20;
    inline constexpr int kFoodHeight = 20;

    inline void InitFoodAssets() {
        for (size_t i = 0; i < kFoodFiles.size(); ++i) {
            const char* path = kFoodFiles[i];
            
            Image img = LoadImage(path);

            if (img.data == nullptr) {
                TraceLog(LOG_ERROR, "Failed to load image: %s", path);
                continue;
            }

            ImageResize(&img, kFoodWidth, kFoodHeight);

            g_FoodTextures[i] = LoadTextureFromImage(img);
        }        
    }
}

// Gate Assets
namespace Assets
{
    inline constexpr std::array<const char*, 1> kGateFiles = {
        "assets/Gate.jpg"
    };

    inline std::array<Texture2D, kGateFiles.size()> g_GateTextures;

    inline constexpr int kGateWidth = 35;
    inline constexpr int kGateHeight = 130;

    inline void InitGateAssets() {
        for (size_t i = 0; i < kGateFiles.size(); ++i) {
            const char* path = kGateFiles[i];
            
            Image img = LoadImage(path);

            if (img.data == nullptr) {
                TraceLog(LOG_ERROR, "Failed to load image: %s", path);
                continue;
            }

            ImageResize(&img, kGateWidth, kGateHeight);

            g_GateTextures[i] = LoadTextureFromImage(img);
        }        
    }
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

