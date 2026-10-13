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

  healthBarTimer = std::max(0.0f, healthBarTimer - deltaTime);

  if (IsDead())
  {
      return;
  }
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
    if (IsDead())
    {
        return;
    }

    DrawRectangleRec(bounds, ORANGE);

    if (healthBarTimer > 0.0f)
    {
        constexpr float barWidth = 40.0f;
        constexpr float barHeight = 6.0f;
        constexpr float gap = 8.0f;

        const float barX =
            bounds.x + bounds.width / 2.0f - barWidth / 2.0f;

        const float barY = bounds.y - gap - barHeight;

        const float healthRatio =
            static_cast<float>(health) / maxHealth;

        // Background represents missing health.
        DrawRectangleRec(
            Rectangle{barX, barY, barWidth, barHeight},
            DARKGRAY
        );

        // Filled portion represents remaining health.
        DrawRectangleRec(
            Rectangle{
                barX,
                barY,
                barWidth * healthRatio,
                barHeight
            },
            GREEN
        );
    }
}

Rectangle EnemyFollower::GetBounds() const
{
    return bounds;
}

void EnemyFollower::TakeDamage(int damage)
{
    if (damage <= 0 || IsDead())
    {
        return;
    }

    health = std::max(0, health - damage);
    healthBarTimer = 2.0f;
}

bool EnemyFollower::IsDead() const
{
    return health == 0;
}

int EnemyFollower::GetHealth() const
{
    return health;
}