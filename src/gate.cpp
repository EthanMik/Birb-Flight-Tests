#include "globals.h"

Gate::Gate(float x, float y, Texture2D* texture)
    : x(x)
    , y(y)
    , texture(texture)
{}

void Gate::Draw() {
    if (!opened) {
        DrawTexture(*texture, x - Assets::kGateWidth / 2, y - Assets::kGateHeight / 2, {255, 255, 255, (unsigned char)alpha});
    }
}

void Gate::Open() {
    // timer.reset();
    // while (timer.elapsed() < 1) {
    //     alpha -= 1;
    // }
    opened = true;
}

void Gate::Close() {
    opened = false;
}