#include "globals.h"

Food::Food(float x, float y, Texture2D* texture)
    : x(x)
    , y(y)
    , texture(texture)
{}

void Food::Draw() {
    DrawTexture(*texture, x - Assets::kFoodWidth / 2, y - Assets::kFoodHeight / 2, {255, 255, 255, 255});
}