#pragma once

#include "globals.h"

class Overlay {
public:
    Overlay();

    void Reset(int startingCash);
    bool ImageButton(Texture2D tex, Vector2 pos);

    void print_map();
    
    void Update();
    void Draw();

private:
    std::array<std::tuple<const char*, Rectangle, std::string>, 14> btnSpecs;
    std::unordered_map<std::string, std::pair<Texture2D, Vector2>> btnTextures;

    std::unordered_map<std::string, Animal*> animals;
    std::string selectedAnimal{};
    Vector2 selectPos;

    void AnimalSelctor();
    void KillAnimalBtn();
    void FlingAnimalBtn();
    void SpeedAnimalBtn();
    void HideAnimalBtn();
};