#include "globals.h"

Map::Map() {}
Map::~Map() {}

void Map::Generate(Rectangle mapRegion, int mapIndex) {
    this->mapRegion = mapRegion;

    // Randomize animal/food/gate positions
    std::shuffle(Animals().begin(), Animals().end(), Random::generate());
    std::shuffle(foods.begin(), foods.end(), Random::generate());
    std::shuffle(gates.begin(), gates.end(), Random::generate());

    // Pull random map from asset paths
    int randomMapIndex = mapIndex < 0 ? Random::get(0, Assets::kMapFiles.size() - 1) : mapIndex;
    Image mapImg = LoadImage(Assets::kMapFiles[randomMapIndex]);
    ImageResize(&mapImg, mapRegion.width, mapRegion.height);
    
    BuildWalls(&mapImg, 7);


    this->mapTexture = LoadTextureFromImage(mapImg);

    UnloadImage(mapImg);
}

void Map::Draw() {
    DrawTexture(mapTexture, mapRegion.x, mapRegion.y, {255, 255, 255, 255});
    for (auto& gate : gates) gate.Draw();
    for (auto& food : foods) food.Draw();
}

void Map::Update() {
    if (ResolveFoodCollisions()) return;
    ResolveWallCollisions();
    ResolveAnimalCollisions();
    for (auto& animal : Animals()) animal.Update();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (auto& animals : Animals()) {
            animals.IncreaseVelocity(1.1);
        }
    }
}

void Map::PlaceFood(float x, float y, size_t count) {
    if (foods.size() <= count) {
        size_t randomFoodIndex = Random::get(0, Assets::g_FoodTextures.size() - 1);
        Food food{x, y, &Assets::g_FoodTextures[randomFoodIndex]};
        foods.push_back(food);
    }
}

void Map::PlaceGate(float x, float y, size_t count) {
    if (gates.size() <= count) {
        size_t randomGateIndex = Random::get(0, Assets::g_GateTextures.size() - 1);
        Gate gate{x, y, &Assets::g_GateTextures[randomGateIndex]};
        gates.push_back(gate);
    }
}

void Map::PlaceAnimal(float x, float y, size_t count) {
    const float INITIAL_ANIMAL_VELOCITY = 2;

    Animals()[count].SetPosition({x, y});
    Animals()[count].SetVelocity(components(Random::get(0, 360), INITIAL_ANIMAL_VELOCITY));
}

void Map::BuildWalls(const Image* mask, int resolution) {
    this->mapResolution = resolution;
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
    size_t foodCount = 0;
    size_t gateCount = 0;
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
            PlaceGate(x, y, gateCount++);
        }
        // Checking for GREEN pixel
        if (c.r == 0 && c.b == 0 && c.g == 255) {
            PlaceFood(x, y, foodCount++);
        }
    }

    UnloadImageColors(px);    

    this->map = map;
}

void Map::ResolveWallCollisions() {
    for (auto& animal : Animals()) {
        segment seg{};

        Vector2 v = animal.GetVelocity();
        int STEP_COUNT = 7;
        Vector2 dv = { v.x / STEP_COUNT, v.y / STEP_COUNT };
    
        for (int i = 0; i < STEP_COUNT; ++i) {
            Vector2 position{animal.GetPosition().x + dv.x, animal.GetPosition().y + dv.y};
            animal.SetPosition(position);
    
            if (CheckWallCollisions(animal.GetPosition(), &seg)) {
                Vector2 p = animal.GetPosition();
                Vector2 q = closest_point_seg(p, seg.a, seg.b);
                Vector2 n = normalize(subtract(p, q));
                float push = Assets::kAnimalRadius - dist(p, q);
                if (push > 0) { 
                    p = add(p, scale(n, push + 0.05f)); 
                    animal.SetPosition({p.x, p.y});
                }
                dv = reflect(dv, n);
                v = reflect(v, n);
                
                animal.SetVelocity({v.x, v.y});
            }
        }
        // if (x < 0 || x > x + w || y < 0 || y < y + h) {
            // Reset position
        // }

    }
}

static bool CollisionAnimalWall(Vector2 center, float radius, Vector2 a, Vector2 b, segment* seg) {
    if (CheckCollisionCircleLine(center, radius, a, b)) {
        if (seg) {
            seg->a = a;
            seg->b = b;
        }
        return true;
    }
    return false;
}

bool Map::CheckWallCollisions(Vector2 animalPosition, segment* seg) {
    if (!map) { return false; }

    int rez = this->mapResolution;
    int w_ = mapRegion.width / rez;
    int h_ = mapRegion.height / rez;

    Vector2 center = animalPosition;
    float radius = Assets::kAnimalRadius;
    
    float lookaheadArea = radius * 4;
    int minX = max(0, (int)floorf((center.x - lookaheadArea) / rez));
    int maxX = min(w_- 2,(int)floorf((center.x + lookaheadArea) / rez));
    int minY = max(0, (int)floorf((center.y - lookaheadArea) / rez));
    int maxY = min(h_ - 2, (int)floorf((center.y + lookaheadArea) / rez));

    for (int y_ = minY; y_ <= maxY; y_++) {
        for (int x_ = minX; x_ < maxX; x_++) {
            int x = x_ * rez;
            int y = y_ * rez;
            Vector2 a = { x, y + rez * 0.5f };
            Vector2 b = { x + rez * 0.5f, y };
            Vector2 c = { x + rez, y + rez * 0.5f };
            Vector2 d = { x + rez * 0.5f, y + rez };

            int c1 = map[y_ * w_ + x_];
            int c2 = map[y_ * w_ + (x_ + 1)];
            int c3 = map[(y_ + 1) * w_ + (x_ + 1)];
            int c4 = map[(y_ + 1) * w_ + x_];
            int idx = c1 * 8 + c2 * 4 + c3 * 2 + c4 * 1;

            switch (idx) {
                case 0: case 15: break;
                case 1:  if (CollisionAnimalWall(center, radius, a, d, seg)) return true; break;
                case 2:  if (CollisionAnimalWall(center, radius, c, d, seg)) return true; break;
                case 3:  if (CollisionAnimalWall(center, radius, a, c, seg)) return true; break;
                case 4:  if (CollisionAnimalWall(center, radius, b, c, seg)) return true; break;
                case 5:  if (CollisionAnimalWall(center, radius, a, b, seg) || CollisionAnimalWall(center, radius, c, d, seg)) return true; break;
                case 6:  if (CollisionAnimalWall(center, radius, b, d, seg)) return true; break;
                case 7:  if (CollisionAnimalWall(center, radius, a, b, seg)) return true; break;
                case 8:  if (CollisionAnimalWall(center, radius, a, b, seg)) return true; break;
                case 9:  if (CollisionAnimalWall(center, radius, b, d, seg)) return true; break;
                case 10: if (CollisionAnimalWall(center, radius, a, d, seg) || CollisionAnimalWall(center, radius, b, c, seg)) return true; break;
                case 11: if (CollisionAnimalWall(center, radius, b, c, seg)) return true; break;
                case 12: if (CollisionAnimalWall(center, radius, a, c, seg)) return true; break;
                case 13: if (CollisionAnimalWall(center, radius, c, d, seg)) return true; break;
                case 14: if (CollisionAnimalWall(center, radius, a, d, seg)) return true; break;
            }
        }
    }
    return false;
}

bool Map::ResolveFoodCollisions() {
    for (auto& animal : Animals()) {
        for (auto& food : foods)
            if (CheckCollisionCircleRec(animal.GetPosition(), animal.GetRadius(), food.Region())) {
                return true;
            }
    }
    return false;
}

void Map::ResolveAnimalCollisions() {
    for (size_t i = 0; i < Animals().size(); ++i) for (size_t j = 0; j < Animals().size(); ++j) {

        Animal& a = Animals()[i];
        Animal& b = Animals()[j];

        if (!CheckCollisionCircles(a.GetPosition(), a.GetRadius(), b.GetPosition(), b.GetRadius())
            || !a.HasCollision() || !b.HasCollision()) continue; 
        
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





static void drawLine(Vector2 a, Vector2 b) { 
    DrawLine(a.x, a.y, b.x, b.y, RED);
}

void Map::test_DrawWalls() {
    if (!map) { return; }
    int rez = mapResolution;
    int w_ = mapRegion.width / rez;
    int h_ = mapRegion.height / rez;

    for (int y_ = 0; y_ < h_ - 1; y_++) {
        for (int x_ = 0; x_ < w_ - 1; x_++) {
            int x = x_ * rez;
            int y = y_ * rez;
            Vector2 a = { x, y + rez * 0.5f };
            Vector2 b = { x + rez * 0.5f, y };
            Vector2 c = { x + rez, y + rez * 0.5f };
            Vector2 d = { x + rez * 0.5f, y + rez };

            int c1 = map[y_ * w_ + x_];
            int c2 = map[y_ * w_ + (x_ + 1)];
            int c3 = map[(y_ + 1) * w_ + (x_ + 1)];
            int c4 = map[(y_ + 1) * w_ + x_];
            int idx = c1 * 8 + c2 * 4 + c3 * 2 + c4 * 1;

            switch (idx) {
                case 0:  break;
                case 1:  drawLine(a, d); break;
                case 2:  drawLine(c, d); break; 
                case 3:  drawLine(a, c); break;                
                case 4:  drawLine(b, c); break; 
                case 5:  drawLine(a, b); drawLine(c, d); break; 
                case 6:  drawLine(b, d); break;
                case 7:  drawLine(a, b); break;
                case 8:  drawLine(a, b); break;
                case 9:  drawLine(b, d); break;
                case 10: drawLine(a, d); drawLine(b, c); break;
                case 11: drawLine(b, c); break;
                case 12: drawLine(a, c); break;
                case 13: drawLine(c, d); break;
                case 14: drawLine(a, d); break;
                case 15: break;
            }
        }
    }
}