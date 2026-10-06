#pragma once

#include "raylib.h"

class Player {
  public:  //contains methods other classes can call
    Player(float x, float y);

    void Update(float deltaTime);
    void Draw() const;

  private: //priate refers to data managed by the class
    Rectangle bounds;
    float speed = 220.0f;
}