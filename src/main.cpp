#include "globals.h"
#include <cstdlib>
#include <enet/enet.h>

int main() {
  // Enet init
  if (enet_initialize() != 0) {
    fprintf(stderr, "Enet init failed");
    return EXIT_FAILURE;
  }

  ENetAddress address;
  ENetHost* server;
  
  address.host = ENET_HOST_ANY;
  address.port = 1234;
  server = enet_host_create(&address, 32, 2, 0, 0);

  if (server == NULL) {
    fprintf(stderr, "Enet server creation failed");
    exit(EXIT_FAILURE);
  }

  // constexpr int screenWidth = 1440;
  // constexpr int screenHeight = 810;
  constexpr int screenWidth = 960;
  constexpr int screenHeight = 540;

  InitWindow(screenWidth, screenHeight, "Birb Flight Tests");
  SetTargetFPS(60);

  Assets::InitAnimalAssets();
  Assets::InitFoodAssets();
  Assets::InitGateAssets();
  Assets::InitOverlayAssets();
  Assets::InitFontAssets();

  Map map{};
  Overlay overlay{};

  map.Generate({0, 60, 640, 480}, 0);

  while (!WindowShouldClose()) {
    map.Update();

    BeginDrawing();
    map.Draw();

    for (auto &animal : Animals()) {
      animal.Draw();
    }

    overlay.Draw();

    EndDrawing();
  }

  CloseWindow();
  atexit(enet_deinitialize);
  enet_host_destroy(server);
}