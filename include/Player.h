#pragma once

#include "raylib.h"
#include <vector>

class Player {
  public:  //contains methods other classes can call
    Player(float x, float y);

    void Update(float deltaTime, const std::vector<Rectangle>& walls);
    void Draw() const;
    Rectangle GetBounds() const;
    void SetPosition(Vector2 position);

  private: //priate refers to data managed by the class
    Rectangle bounds;
    float speed = 220.0f;
};