#include "raylib.h"
#include "Player.h"
#include "Room.h"

#include <algorithm>
#include <cmath>
#include <utility>
#include <string>
#include <optional>

int main()
{
  constexpr int screenWidth = 960;
  constexpr int screenHeight = 640; //constexpr value to be known at compile time and value never changes

  InitWindow(screenWidth, screenHeight, "Dungeon Crawler"); //initalizing window + setting target fps
  SetTargetFPS(60);

  Player player{
        screenWidth / 2.0f - 16.0f,
        screenHeight / 2.0f - 16.0f
    };

  Room room{0};

  if (!room.LoadLevel("assets/levels/room1.txt"))
  {
      TraceLog(LOG_ERROR, "Could not load room1.txt");
      CloseWindow();
      return 1;
  }

  while(!WindowShouldClose()) { //keeps window open and game loop running

    const float deltaTime = GetFrameTime();
    room.Draw();

    player.Update(deltaTime, room.GetWalls());

    std::optional<Door> enteredDoor;

    for (const Door& door : room.GetDoors()) {

      if(CheckCollisionRecs(player.GetBounds(), door.bounds)) {

        enteredDoor = door;
        break;
      }
    }

    // Switch rooms if a door was found.
    if (enteredDoor.has_value())
    {
        const Door& door = enteredDoor.value();

        const std::string filePath =
            "assets/levels/room" +
            std::to_string(door.destinationRoom) +
            ".txt";

        Room nextRoom{door.destinationRoom};

        if (nextRoom.LoadLevel(filePath))
        {
            room = std::move(nextRoom);
            player.SetPosition(door.destinationSpawn);
        }
        else
        {
            TraceLog(
                LOG_WARNING,
                "Could not load room: %s",
                filePath.c_str()
            );
        }
    }

    BeginDrawing();

    ClearBackground(Color{22, 22, 30, 225});

    player.Draw();
    DrawText("Dungeon Crawler", 20, 20, 28, RAYWHITE);
    DrawText("WASD to move | ESC to quit", 20, 56, 20, GRAY);
    DrawFPS(screenWidth - 100, 20);

    EndDrawing();


  }

  CloseWindow();

  return 0;
}