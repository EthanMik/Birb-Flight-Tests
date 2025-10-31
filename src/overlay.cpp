#include "globals.h"

Overlay::Overlay() {
    btnSpecs = {{
        { "assets/Overlays/SudokuButton.png", Rectangle{650, 245, 128, 43}, "SudokuButton" },
        { "assets/Overlays/FlingButton.png", Rectangle{650, 326, 128, 43}, "FlingButton" },
        { "assets/Overlays/SpeedButton.png", Rectangle{650, 405, 128, 43}, "SpeedButton" },
        { "assets/Overlays/HideButton.png", Rectangle{650, 482, 128, 43}, "HideButton" },

        { "assets/Birbs/Gray.png", Rectangle{635, 70, 60, 60}, "Gray" },
        { "assets/Birbs/Blue.png", Rectangle{685, 70, 60, 60}, "Blue" },
        { "assets/Birbs/Yellow.png", Rectangle{735, 70, 60, 60}, "Yellow" },
        { "assets/Birbs/Red.png", Rectangle{635, 120, 60, 60}, "Red" },
        { "assets/Birbs/White.png", Rectangle{685, 120, 60, 60}, "White" },
        { "assets/Birbs/Brown.png", Rectangle{735, 120, 60, 60}, "Brown" },
        { "assets/Birbs/Pink.png", Rectangle{635, 170, 60, 60}, "Pink" },
        { "assets/Birbs/Green.png", Rectangle{685, 170, 60, 60}, "Green" },
        { "assets/Birbs/Black.png", Rectangle{735, 170, 60, 60}, "Black" }
    }};

    for (size_t i = 0; i < btnSpecs.size(); ++i) {
        const char* path = get<0>(btnSpecs[i]);
        Rectangle bound = get<1>(btnSpecs[i]);
        std::string name = get<2>(btnSpecs[i]);
        Image img = LoadImage(path);

        if (img.data == nullptr) {
            TraceLog(LOG_ERROR, "Failed to load image: %s", path);
            continue;
        }
        ImageResize(&img, bound.width, bound.height);
        btnTextures[name] = {LoadTextureFromImage(img), {bound.x, bound.y}};
        UnloadImage(img);
    }
}

bool Overlay::ImageButton(Texture2D tex, Vector2 pos) {
    Rectangle boundingBox = { pos.x, pos.y, tex.width, tex.height };
    Vector2 mousePos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mousePos, boundingBox);
    bool pressed = hover && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool clicked = hover && pressed && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    Color tint = WHITE;
    if (hover) tint = (Color){ 200, 200, 200, 255 };
    if (pressed) tint = (Color){ 170, 170, 170, 255 };

    DrawTexture(tex, pos.x, pos.y, tint);

    return clicked;
}

void Overlay::Draw() {
    for (const auto& asset : Assets::g_OverlayTextures) {
        DrawTexture(asset.first, asset.second.x, asset.second.y, WHITE);
    }
    KillAnimalBtn();
    FlingAnimalBtn();
    SpeedAnimalBtn();
    HideAnimalBtn();
    AnimalSelctor();
}

void Overlay::AnimalSelctor() {
    auto gray = btnTextures["Gray"];
    auto blue = btnTextures["Blue"];
    auto yellow = btnTextures["Yellow"];
    auto red = btnTextures["Red"];
    auto white = btnTextures["White"];
    auto brown = btnTextures["Brown"];
    auto pink = btnTextures["Pink"];
    auto green = btnTextures["Green"];
    auto black = btnTextures["Black"];
    
    if (ImageButton(gray.first, gray.second)) {}
    if (ImageButton(blue.first, blue.second)) {}
    if (ImageButton(yellow.first, yellow.second)) {}
    if (ImageButton(red.first, red.second)) {}
    if (ImageButton(white.first, white.second)) {}
    if (ImageButton(brown.first, brown.second)) {}
    if (ImageButton(pink.first, pink.second)) {}
    if (ImageButton(green.first, green.second)) {}
    if (ImageButton(black.first, black.second)) {}
}

void Overlay::KillAnimalBtn() {
    auto btn = btnTextures["SudokuButton"];
    if (ImageButton(btn.first, btn.second)) {

    }
}

void Overlay::FlingAnimalBtn() {
    auto btn = btnTextures["FlingButton"];
    if (ImageButton(btn.first, btn.second)) {
    
    }
}

void Overlay::SpeedAnimalBtn() {
    auto btn = btnTextures["SpeedButton"];
    if (ImageButton(btn.first, btn.second)) {
    
    }
}
void Overlay::HideAnimalBtn() {
    auto btn = btnTextures["HideButton"];
    if (ImageButton(btn.first, btn.second)) {
    
    }    
}