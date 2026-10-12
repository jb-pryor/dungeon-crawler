#include "EnemyFollower.h"

#include <algorithm>
#include <cmath>

EnemyFollower::EnemyFollower(float x, float y)
    : bounds{x, y, 32.0f, 32.0f}
{
}

void EnemyFollower::Update(
    float deltaTime,
    Rectangle playerBounds,
    const std::vector<Rectangle>& walls)
{
    // Find the center of the enemy and player.
    const Vector2 enemyCenter{
        bounds.x + bounds.width / 2.0f,
        bounds.y + bounds.height / 2.0f
    };

    const Vector2 playerCenter{
        playerBounds.x + playerBounds.width / 2.0f,
        playerBounds.y + playerBounds.height / 2.0f
    };

    // Point from the enemy toward the player.
    Vector2 direction{
        playerCenter.x - enemyCenter.x,
        playerCenter.y - enemyCenter.y
    };

    const float distance = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    if (distance == 0.0f)
    {
        return;
    }

    // Normalize the direction.
    direction.x /= distance;
    direction.y /= distance;

    // Avoid moving past the player's center.
    const float movement = std::min(speed * deltaTime, distance);

    // Move horizontally and check walls.
    const float previousX = bounds.x;
    bounds.x += direction.x * movement;

    for (const Rectangle& wall : walls)
    {
        if (CheckCollisionRecs(bounds, wall))
        {
            bounds.x = previousX;
            break;
        }
    }

    // Move vertically and check walls.
    const float previousY = bounds.y;
    bounds.y += direction.y * movement;

    for (const Rectangle& wall : walls)
    {
        if (CheckCollisionRecs(bounds, wall))
        {
            bounds.y = previousY;
            break;
        }
    }
}

void EnemyFollower::Draw() const
{
    DrawRectangleRec(bounds, ORANGE);
}