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

  private:
    Rectangle bounds;
    float speed = 180.0f;
};