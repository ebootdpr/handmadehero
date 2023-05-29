#include "raylib.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <valarray>

struct Stats {
  int max_speed;
  int h;
  int r;
};
struct Entity {
  int id;
  int x;
  int y;
  int velx;
  int vely;
  int w;
  int h;
  int r;
  Stats stats;
};

void System_Physics_FirstNewtonLaw(Entity &P) {
  P.x += P.velx;
  P.y += P.vely;
}

void System_Physics_CollideWitWorldBorders(Entity &P, int x_max, int y_max) {
  if (P.x > x_max || P.x < 20) {
    P.velx = -P.velx * 0.68;
    P.x += 2 * P.velx;
  }
  if (P.y > y_max || P.y < 20) {
    P.vely = -P.vely * 0.68;
    P.y += 2 * P.vely;
  }
}

void System_Physics_CollideWithEntities(Entity *Players[], int lenP,
                                        Entity *Obstacles[], int lenO) {

  for (int i = 0; i < lenP; i++) {
    Entity P = *Players[i];
    for (int j = 0; j < lenO; j++) {
      Entity O = *Obstacles[j];
      // int d = std::sqrt(std::pow(P.x - O.x, 2) + std::pow(P.y - O.y, 2));
      if (P.y + P.r > O.y && P.y - P.r < O.y + O.h) {
        if (P.x + P.r > O.x && P.x - P.r < O.x + O.w) {

          // COLLIDING!!!
          // STRATEGY, MEASURE FROM THE CENTER AND DITCH IT!
          // Too bad!
          // Better: 8 ifs conditionals
          if (P.x > O.x + O.w - 1) { // RIGHT
            Players[i]->velx = -P.velx * 0.8;
            Players[i]->x = P.x + P.velx;
          } else if (P.y > O.y + O.h - 1) { // BOTTOM
            Players[i]->vely = -P.vely * 0.8;
            Players[i]->y = P.y - P.vely;
          } else if (P.x < O.x + 1) { // LEFT
            Players[i]->velx = -P.velx * 0.8;
            Players[i]->x = P.x - P.velx;
          } else if (P.y < O.y + 1) { // TOP
            Players[i]->vely = -P.vely * 0.8;
            Players[i]->y = P.y - P.vely;
          }
        }
      }
    }
  }
}

int main() {
  // Determin the Game Window Width and Height
  const int screenWidth = 800;
  const int screenHeight = 450;
  // Initialize the Window
  InitWindow(screenWidth, screenHeight, "My Game");

  // Setting the Frames Per Second
  SetTargetFPS(60);

  // create entities

  Entity Player = *new Entity{};
  Player.id = 0;
  Player.x = 50;
  Player.y = 50;
  Player.r = 24;
  Player.h = Player.w = 2 * Player.r;
  Player.stats.max_speed = 5;

  Entity Obstacle = *new Entity{};
  Obstacle.id = 1;
  Obstacle.h = 50;
  Obstacle.w = 100;
  Obstacle.x = 150;
  Obstacle.y = 150;
  Obstacle.r = std::sqrt(std::pow(Obstacle.h, 2) + std::pow(Obstacle.w, 2));

  Entity wall1 = *new Entity{};
  wall1.id = 1;
  wall1.h = 50;
  wall1.w = 600;
  wall1.x = 150;
  wall1.y = 350;
  wall1.r = std::sqrt(std::pow(wall1.h, 2) + std::pow(wall1.w, 2));
  std::array<Entity, 2> asd = {Obstacle, wall1};

  Entity *scene_obstacles[2] = {&Obstacle, &wall1};
  int max_o = 2;
  Entity *scene_players[1] = {&Player};
  int max_p = 1;
  // The Game Loop
  while (!WindowShouldClose() /*WindowShouldClose returns true if esc is clicked and closes the window*/) {

    // Setup Canvas
    BeginDrawing();
    // Clear canvas to a specific color to avoid flicker
    ClearBackground(RAYWHITE);

    // Here goes all the Game Logic
    // Circle Movement
    if (IsKeyDown(KEY_W) && Player.y > Player.r) {
      Player.vely -= 1;
    }
    if (IsKeyDown(KEY_A) && Player.x > Player.r) {
      Player.velx -= 1;
    }
    if (IsKeyDown(KEY_S) && Player.y < screenHeight - Player.r) {
      Player.vely += 1;
    }
    if (IsKeyDown(KEY_D) && Player.x < screenWidth - Player.r) {
      Player.velx += 1;
    }
    std::clamp(Player.velx, -Player.stats.max_speed, Player.stats.max_speed);

    System_Physics_FirstNewtonLaw(Player);
    System_Physics_CollideWitWorldBorders(Player, screenWidth, screenHeight);
    System_Physics_CollideWithEntities(scene_players, max_p, scene_obstacles,
                                       max_o);
    // Draws a Circle in the Canvas(X Axis, Y Axis, Radius, Color)
    DrawCircle(Player.x, Player.y, Player.r, BLUE);
    DrawRectangle(Obstacle.x, Obstacle.y, Obstacle.w, Obstacle.h, RED);
    DrawRectangle(wall1.x, wall1.y, wall1.w, wall1.h, RED);

    // teardown Canvas
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
