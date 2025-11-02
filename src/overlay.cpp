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
        { "assets/Birbs/Black.png", Rectangle{735, 170, 60, 60}, "Black" },

        { "assets/Overlays/Glow.png", Rectangle{635, 70, 60, 60}, "Glow" }
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

    Reset(1000);
}

bool Overlay::ImageButton(Texture2D tex, Vector2 pos) {
    return ImageButton(tex, {}, pos);
}

bool Overlay::ImageButton(Texture2D tex, Text text, Vector2 pos) {
    Rectangle boundingBox = { pos.x, pos.y, (float)tex.width, (float)tex.height };
    Vector2 mousePos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mousePos, boundingBox);
    bool pressed = hover && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool clicked = hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    Color tint = WHITE;
    if (hover) tint = (Color){ 200, 200, 200, 255 };
    if (pressed) tint = (Color){ 170, 170, 170, 255 };
    bool updateTint = hover || pressed;

    DrawTexture(tex, pos.x, pos.y, tint);
    
    if (text.text != "") {
        float centerX = pos.x + tex.width / 2;
        float centerY = pos.y + tex.height / 2;

        Vector2 textSize = MeasureTextEx(*text.font, text.text.c_str(), text.size, text.spacing);
        Vector2 textPos = {
            centerX - textSize.x / 2.0f - text.centerOffset.x,
            centerY - textSize.y / 2.0f - text.centerOffset.y
        };
        Color textTint = text.tint; 
        if (updateTint) textTint = tint;
        DrawTextEx(*text.font, text.text.c_str(), textPos, text.size, text.spacing, textTint);
    }

    return clicked;
}

void Overlay::Reset(int startingCash) {
    selectPos = btnTextures["Glow"].second;
    selectedAnimal = "Gray";
}

void Overlay::Draw() {
    for (const auto& asset : Assets::g_OverlayTextures) {
        DrawTexture(asset.first, asset.second.x, asset.second.y, WHITE);
    }
    DrawTextEx(Assets::Fonts::pixelifySans24px, "$1000", {680, 14}, 36, 2, WHITE);

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
    
    if (ImageButton(gray.first, gray.second)) { selectPos = gray.second; selectedAnimal = "Gray"; }
    if (ImageButton(blue.first, blue.second)) { selectPos = blue.second; selectedAnimal = "Blue"; }
    if (ImageButton(yellow.first, yellow.second)) { selectPos = yellow.second; selectedAnimal = "Yellow"; }
    if (ImageButton(red.first, red.second)) { selectPos = red.second; selectedAnimal = "Red"; }
    if (ImageButton(white.first, white.second)) { selectPos = white.second; selectedAnimal = "White"; }
    if (ImageButton(brown.first, brown.second)) { selectPos = brown.second; selectedAnimal = "Brown"; }
    if (ImageButton(pink.first, pink.second)) { selectPos = pink.second; selectedAnimal = "Pink"; }
    if (ImageButton(green.first, green.second)) { selectPos = green.second; selectedAnimal = "Green"; }
    if (ImageButton(black.first, black.second)) { selectPos = black.second; selectedAnimal = "Black"; }

    auto glow = btnTextures["Glow"];
    DrawTexture(glow.first, selectPos.x, selectPos.y, WHITE);
}

void Overlay::KillAnimalBtn() {
    auto btn = btnTextures["SudokuButton"];
    if (ImageButton(btn.first, Text{ &Assets::pixelifySans24px, "Kaboom", {3, -3}, 24, 2, {230, 230, 230, 255}}, btn.second)) {
        Assets::g_animalsMap[selectedAnimal]->Kill(5);
    }
}

void Overlay::FlingAnimalBtn() {
    auto btn = btnTextures["FlingButton"];
    if (ImageButton(btn.first, Text{ &Assets::pixelifySans24px, "Fling", {3, -3}, 24, 2, {230, 230, 230, 255}}, btn.second)) {
        Assets::g_animalsMap[selectedAnimal]->Fling(3, .3);
    }
}

void Overlay::SpeedAnimalBtn() {
    auto btn = btnTextures["SpeedButton"];
    if (ImageButton(btn.first, Text{ &Assets::pixelifySans24px, "Speed", {3, -3}, 24, 2, {230, 230, 230, 255}}, btn.second)) {
        Assets::g_animalsMap[selectedAnimal]->IncreaseVelocity(2, 10);
    }
}
void Overlay::HideAnimalBtn() {
    auto btn = btnTextures["HideButton"];
    if (ImageButton(btn.first, Text{ &Assets::pixelifySans24px, "Hide", {3, -3}, 24, 2, {230, 230, 230, 255}}, btn.second)) {
        Assets::g_animalsMap[selectedAnimal]->NonColliding(10);
    }    
}