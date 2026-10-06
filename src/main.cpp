#include "raylib.h"

#include <algorithm>
#include <cmath>

int main()
{
  constexpr int screenWidth = 960;
  constexpr int screenHeight = 640; //constexpr value to be known at compile time and value never changes

  InitWindow(screenWidth, screenHeight, "Dungeon Crawler"); //initalizing window + setting target fps
  SetTargetFPS(60);

  Rectangle player{
    screenWidth / 2.0f - 16.0f, //creating the player
    screenHeight / 2.0f - 16.0f,
    32.0f,
    32.0f
  };

  constexpr float playerSpeed = 220.0f;

  while(!WindowShouldClose()) { //keeps window open and game loop running

    const float deltaTime = GetFrameTime();

    Vector2 direction{0.0f, 0.0f};

    if(IsKeyDown(KEY_W)) direction.y -= 1.0f;
    if(IsKeyDown(KEY_S)) direction.y += 1.0f; //key input for direction movement 
    if(IsKeyDown(KEY_A)) direction.x -= 1.0f;
    if(IsKeyDown(KEY_D)) direction.x += 1.0f;

    const float length = std::sqrt( //nomralization for diagonal movement
      direction.x * direction.x +
      direction.y * direction.y  
    );

    if (length > 0.0f) {
      direction.x /= length; //if only one direction sqrt of 1 is 1 dividing by 1 else
      direction.y /= length;
    }

    player.x += direction.x * playerSpeed * deltaTime;
    player.y += direction.y * playerSpeed * deltaTime; //always times by deltatime so it runs same on everyones computer

    player.x = std::clamp(
      player.x, 0.0f, screenWidth - player.width //clamps plauer x to stay within screen
    );

    player.y = std::clamp(
      player.y, 0.0f, screenHeight - player.height //same clamp for y
    );

    BeginDrawing();

    ClearBackground(Color{22, 22, 30, 225});

    DrawRectangleRec(player, SKYBLUE);
    DrawText("Dungeon Crawler", 20, 20, 28, RAYWHITE);
    DrawText("WASD to move | ESC to quit", 20, 56, 20, GRAY);
    DrawFPS(screenWidth - 100, 20);

    EndDrawing();


  }

  CloseWindow();

  return 0;
}