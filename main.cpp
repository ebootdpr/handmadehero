#include "raylib.h"

int main() {
  // Determin the Game Window Width and Height
  const int screenWidth = 800;
  const int screenHeight = 450;
  // Initialize the Window
  InitWindow(screenWidth, screenHeight, "My Game");

  // Setting the Frames Per Second
  SetTargetFPS(60);

  // The Game Loop
  while (!WindowShouldClose() /*WindowShouldClose returns true if esc is clicked and closes the window*/) {

    // Setup Canvas
    BeginDrawing();
    // Clear canvas to a specific color to avoid flicker
    ClearBackground(RAYWHITE);

    // Here goes all the Game Logic

    // teardown Canvas
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
