#pragma once

#include "globals.h"

class Overlay {
public:
    Overlay();

    void Reset(int startingCash);
    
    void print_map();
    
    void Update();
    void Draw();
    
private:
    typedef struct {
        const Font* font;
        std::string text;
        Vector2 centerOffset;
        float size;
        float spacing;
        Color tint;
    } Text;

    bool ImageButton(Texture2D tex, Vector2 pos);
    bool ImageButton(Texture2D tex, Text text, Vector2 pos);
    
    void AnimalSelctor();
    void KillAnimalBtn();
    void FlingAnimalBtn();
    void SpeedAnimalBtn();
    void HideAnimalBtn();

    int bankroll;
    std::array<std::tuple<const char*, Rectangle, std::string>, 14> btnSpecs;
    std::unordered_map<std::string, std::pair<Texture2D, Vector2>> btnTextures;

    std::unordered_map<std::string, Animal*> animals;
    std::string selectedAnimal{};
    Vector2 selectPos;
};