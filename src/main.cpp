#include "raylib.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <math.h>
#include <valarray>

struct stats {
  int max_speed;
  int h;
  int r;
};
struct v2 {
  float x;
  float y;
};
float dot_product(v2 a, v2 b) { return a.x * a.x + b.x + b.x; }

void add(v2 a, v2 *b) {
  b->x += a.x;
  b->y += a.y;
}

void sub(v2 a, v2 *b) {
  b->x -= a.x;
  b->y -= a.y;
}

float module(v2 a) { return sqrtf(a.x * a.x + a.y * a.y); }

float cos_angle(v2 u, v2 v) {
  return (dot_product(u, v)) / (module(u) * module(v));
}

v2 dir(v2 u) {
  v2 direction;
  float mod = module(u);
  direction.x = u.x / mod;
  direction.y = u.y / mod;
  return direction;
}

struct Entity {
  int id;
  v2 pos;
  v2 vel;
  v2 acc;
  float mass;

  int w;
  int h;
  int r;
  stats stt;
};

void System_Physics_FirstNewtonLaw(Entity *P) { add(P->vel, &P->pos); }

void System_Physics_CollideWitWorldBorders(Entity &P, int x_max, int y_max) {

  if (P.pos.x > x_max || P.pos.x < 20) {
    P.vel.x = -P.vel.x * 0.68;
    P.pos.x += 2 * P.vel.x;
  }
  if (P.pos.y > y_max || P.pos.y < 20) {
    P.vel.y = -P.vel.y * 0.68;
    P.pos.y += 2 * P.vel.y;
  }
}

void System_Physics_CollideWithEntities(Entity *Players[], int lenP,
                                        Entity *Obstacles[], int lenO) {

  for (int i = 0; i < lenP; i++) {
    Entity P = *Players[i];
    for (int j = 0; j < lenO; j++) {
      Entity O = *Obstacles[j];
      if (P.pos.y + P.r > O.pos.y - 5 && P.pos.y - P.r < O.pos.y + O.h + 5) {
        if (P.pos.x + P.r > O.pos.x - 5 && P.pos.x - P.r < O.pos.x + O.w + 5) {

          int d1 = std::sqrt(std::pow(P.pos.x - O.pos.x, 2) + std::pow(P.pos.y - O.pos.y, 2));
          int d2 = std::sqrt(std::pow(P.pos.x - (O.pos.x + O.w), 2) +
                             std::pow(P.pos.y - O.pos.y, 2));
          int d3 = std::sqrt(std::pow(P.pos.x - O.pos.x, 2) +
                             std::pow(P.pos.y - (O.pos.y + O.h), 2));
          int d4 = std::sqrt(std::pow(P.pos.x - (O.pos.x + O.w), 2) +
                             std::pow(P.pos.y - (O.pos.y + O.h), 2));
          /* int dir1 = Players[i]->vel.x = */
              // COLLIDING!!!
          DrawCircle((int)P.pos.x, (int)P.pos.y, (int)2 * P.r, MAGENTA);
          
          if (P.pos.x > O.pos.x + O.w) { // RIGHT
            Players[i]->vel.x = -P.vel.x * 0.8;
            Players[i]->pos.x = P.pos.x - P.vel.x;
          } else if (P.pos.y > O.pos.y + O.h) { // BOTTOM
            Players[i]->vel.y = -P.vel.y * 0.8;
            Players[i]->pos.y = P.pos.y - P.vel.y;
          } else if (P.pos.x < O.pos.x) { // LEFT
            Players[i]->vel.x = -P.vel.x * 0.8;
            Players[i]->pos.x = P.pos.x - P.vel.x;
          } else if (P.pos.y < O.pos.y) { // TOP
            Players[i]->vel.y = -P.vel.y * 0.8;
            Players[i]->pos.y = P.pos.y - P.vel.y;
          }
        }
      }
    }
  }
}
int current_id = 0;
Entity create_entity(int width,int height) {
  Entity E = *new Entity();
  E.id = current_id++;
  E.h = height;
  E.w = width;
  E.pos.x = std::rand()/6000000;
  E.pos.y = std::rand()/6000000;
  E.r = std::sqrt(std::pow(E.h, 2) + std::pow(E.w, 2));
  return E;
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
  Player.pos.x = 50;
  Player.pos.y = 50;
  Player.r = 24;
  Player.h = Player.w = 2 * Player.r;
  Player.stt.max_speed = 5;

  Entity Obstacle = create_entity(50,200);
    Entity wall1 =  create_entity(500,20);
    Entity wall2 =  create_entity(500,20);
    Entity wall3 =  create_entity(500,20);


  Entity *scene_obstacles[4] = {&Obstacle, &wall1,&wall2,&wall3};
  int max_o = 4;
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
    if (IsKeyDown(KEY_W) && Player.pos.y > Player.r) {
      Player.vel.y -= 1;
    }
    if (IsKeyDown(KEY_A) && Player.pos.x > Player.r) {
      Player.vel.x -= 1;
    }
    if (IsKeyDown(KEY_S) && Player.pos.y < screenHeight - Player.r) {
      Player.vel.y += 1;
    }
    if (IsKeyDown(KEY_D) && Player.pos.x < screenWidth - Player.r) {
      Player.vel.x += 1;
    }
    /* std::clamp(Player.vel.x, -Player.stt.max_speed, Player.stt.max_speed);
    std::clamp() */
    System_Physics_FirstNewtonLaw(&Player);
    System_Physics_CollideWitWorldBorders(Player, screenWidth, screenHeight);
    System_Physics_CollideWithEntities(scene_players, max_p, scene_obstacles,
                                       max_o);
    // Draws a Circle in the Canvas(X Axis, Y Axis, Radius, Color)
    DrawCircle(static_cast<int>(Player.pos.x), Player.pos.y, Player.r, BLUE);
    for ( int i=0;i<max_o;i++){
      DrawRectangle(scene_obstacles[i]-> pos.x, scene_obstacles[i]-> pos.y, scene_obstacles[i]-> w, scene_obstacles[i]-> h, RED);
    }

    // teardown Canvas
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
