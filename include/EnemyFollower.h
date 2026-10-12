#pragma once

#include "raylib.h"
#include <vector>

class EnemyFollower {

  public:
    EnemyFollower(float x, float y);
    void Update(
        float deltaTime,
        Rectangle playerBounds,
        const std::vector<Rectangle>& walls
    );
    void Draw() const;
    Rectangle GetBounds() const;

  private:
    Rectangle bounds;
    float speed = 120.0f;
};