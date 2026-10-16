#include "raylib.h"
#include "Player.h"
#include "Room.h"
#include "EnemyFollower.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

Vector2 GetArrivalPosition(
    const Door& door,
    Rectangle player,
    int roomWidth,
    int roomHeight)
{
    constexpr float borderSize = 40.0f;
    constexpr float gap = 2.0f;

    Vector2 arrival{player.x, player.y};

    if (door.bounds.x <= 0.0f)
    {
        // Left door -> right side.
        arrival.x =
            roomWidth - borderSize - player.width - gap;
    }
    else if (door.bounds.x + door.bounds.width >= roomWidth)
    {
        // Right door -> left side.
        arrival.x = borderSize + gap;
    }
    else if (door.bounds.y <= 0.0f)
    {
        // Top door -> bottom side.
        arrival.y =
            roomHeight - borderSize - player.height - gap;
    }
    else if (door.bounds.y + door.bounds.height >= roomHeight)
    {
        // Bottom door -> top side.
        arrival.y = borderSize + gap;
    }

    return arrival;
}

int main()
{
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 640;

    InitWindow(screenWidth, screenHeight, "Dungeon Crawler");
    SetTargetFPS(60);

    Player player{
        screenWidth / 2.0f - 16.0f,
        screenHeight / 2.0f - 16.0f
    };

    Room room{1};

    // Preserve each room's enemies between visits.
    std::unordered_map<int, std::vector<EnemyFollower>> enemiesByRoom;

    if (!room.LoadLevel("assets/levels/room1.txt"))
    {
        TraceLog(LOG_ERROR, "Could not load room1.txt");
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        // Update player movement, aiming, and attacks.
        player.Update(deltaTime, room.GetWalls());

        // Detect doors before attempting a room transition.
        std::optional<Door> enteredDoor;

        if (!player.IsDead())
        {
            for (const Door& door : room.GetDoors())
            {
                if (CheckCollisionRecs(
                        player.GetBounds(),
                        door.bounds))
                {
                    enteredDoor = door;
                    break;
                }
            }
        }

        // Load the destination room.
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
                const Vector2 arrival = GetArrivalPosition(
                    door,
                    player.GetBounds(),
                    screenWidth,
                    screenHeight
                );

                room = std::move(nextRoom);
                player.SetPosition(arrival);
                player.CancelAttack();
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

        // Spawn this room's enemies only on its first visit.
        const int currentRoomId = room.GetId();

        if (enemiesByRoom.find(currentRoomId) ==
            enemiesByRoom.end())
        {
            std::vector<EnemyFollower> spawnedEnemies;

            for (const Vector2& spawn : room.GetEnemySpawns())
            {
                spawnedEnemies.emplace_back(spawn.x, spawn.y);
            }

            enemiesByRoom.emplace(
                currentRoomId,
                std::move(spawnedEnemies)
            );
        }

        auto& currentEnemies = enemiesByRoom.at(currentRoomId);

        // Only the active room's enemies participate in gameplay.
        if (!player.IsDead())
        {
            for (EnemyFollower& enemy : currentEnemies)
            {
                if (enemy.IsDead())
                {
                    continue;
                }

                enemy.Update(
                    deltaTime,
                    player.GetBounds(),
                    room.GetWalls()
                );

                if (player.DidAttackThisFrame() &&
                    player.AttackHits(enemy.GetBounds()) &&
                    player.HasClearAttackPath(
                        enemy.GetBounds(),
                        room.GetWalls()))
                {
                    enemy.TakeDamage(25);
                }

                if (!enemy.IsDead() &&
                    CheckCollisionRecs(
                        player.GetBounds(),
                        enemy.GetBounds()))
                {
                    player.TakeDamage(10);
                }

                if (player.IsDead())
                {
                    break;
                }
            }
        }

        // All drawing belongs between these two calls.
        BeginDrawing();

        ClearBackground(Color{22, 22, 30, 255});

        room.Draw();

        for (const EnemyFollower& enemy : currentEnemies)
        {
            enemy.Draw(player.IsDead());
        }

        if (!player.IsDead())
        {
            player.Draw();
        }

        DrawText("Dungeon Crawler", 20, 20, 28, RAYWHITE);

        DrawText(
            "WASD to move | Left click to attack | ESC to quit",
            20,
            56,
            18,
            GRAY
        );

        DrawText(
            TextFormat("Health: %d", player.GetHealth()),
            800,
            20,
            24,
            WHITE
        );

        if (player.IsDead())
        {
            DrawText("Game Over", 360, 280, 40, RED);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}