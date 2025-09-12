#include "globals.h"

Map::Map() {}
Map::~Map() {}

void Map::Generate(Rectangle mapRegion) {
    this->mapRegion = mapRegion;

    int randomMapIndex = Random::get(0, assets::kMapFiles.size() - 1);
    Image mapImg = LoadImage(assets::kMapFiles[randomMapIndex]);
    ImageResize(&mapImg, mapRegion.width, mapRegion.height);
    
    BuildWalls(&mapImg, 7);

    this->mapTexture = LoadTextureFromImage(mapImg);
    UnloadImage(mapImg);

}

void Map::Draw() {
    DrawTexture(mapTexture, mapRegion.x, mapRegion.y, {255, 255, 255, 255});
}

void Map::Update() {
    
}

void Map::PlaceAnimal(float x, float y, size_t count) {
    const float INITIAL_ANIMAL_VELOCITY = 10;

    Animals()[count].SetPosition({x, y});
    Animals()[count].SetVelocity(components(Random::get(0, 360), INITIAL_ANIMAL_VELOCITY));
}

void Map::BuildWalls(const Image* mask, int resolution) {
    int w = mask->width;
    int h = mask->height;
    int w_ = w / resolution;
    int h_ = h / resolution;

    Color* px = LoadImageColors(*mask);
    if (!px) { return; }

    // Building Collision walls at set resolution. 
    unsigned char* map = (unsigned char*)malloc(w_ * h_);
    for (int y_ = 0; y_ < h_; y_++) for (int x_ = 0; x_ < w_; x_++) {
        int x = x_ * resolution;
        int y = y_ * resolution;
        Color c = px[y * w + x];
        map[y_ * w_ + x_] = (c.r > 15) ? 0 : 1;
        // solid[y_ * w_ + x_] = (c.a > alphaThreshold) ? 0 : 1;
        
    }

    size_t animalCount = 0;
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
        Color c = px[y * w + x];
        // Checking for RED pixel
        if (c.r == 255 && c.b == 0 && c.g == 0) {
            if (animalCount < Animals().size()) {
                PlaceAnimal(x, y, animalCount++);
            }
        }
        // Checking for BLUE pixel
        if (c.r == 0 && c.b == 255 && c.g == 0) {
            // Create Gate (Window)
        }
        // Checking for GREEN pixel
        if (c.r == 0 && c.b == 0 && c.g == 255) {
            // Create Seeds
        }
    }

    UnloadImageColors(px);    

    this->map = map;
}

void Map::ResolveWallCollisions() {

}

void Map::CheckWallCollisions() {

}

void Map::ResolveAnimalCollisions() {
    for (size_t i = 0; i < Animals().size(); ++i) for (size_t j = 0; j < Animals().size(); ++j) {

        Animal& a = Animals()[i];
        Animal& b = Animals()[j];

        if (!CheckCollisionCircles(a.GetPosition(), a.radius, b.GetPosition(), b.radius)) continue; 
        
        Vector2 velA = a.GetVelocity();
        Vector2 velB = b.GetVelocity();
        Vector2 posA = a.GetPosition();
        Vector2 posB = b.GetPosition();
        
        Vector2 line = subtract(posB, posA);
        Vector2 normal = normalize(line);
    
        float approach = dot(subtract(velB, velA), normal);
        if (approach >= 0.0f) continue;  
    
        a.SetVelocity(reflect(velA, normal));
        b.SetVelocity(reflect(velB, normal));
    }
}

