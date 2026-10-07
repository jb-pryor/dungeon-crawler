#include "Player.h"

#include <algorithm>
#include <cmath>

Player::Player(float x, float y)
  : bounds{x, y, 32.0, 32.0}
{

}

void Player::Update(float deltaTime, const std::vector<Rectangle>& walls)
{

  Vector2 direction{0.0f, 0.0f};

  if(IsKeyDown(KEY_W)) direction.y -= 1.0f;
  if(IsKeyDown(KEY_S)) direction.y += 1.0f;
  if(IsKeyDown(KEY_A)) direction.x -= 1.0f;
  if(IsKeyDown(KEY_D)) direction.x += 1.0f;

  const float length = std::sqrt(
    direction.x * direction.x +
    direction.y * direction.y
  );

  if(length > 0.0f) {

    direction.x /= length;
    direction.y /= length;  //normalization for diagonal movement. 
  }

  // Try horizontal movement.
  const float previousX = bounds.x;

  bounds.x += direction.x * speed * deltaTime;

  for (const Rectangle& wall : walls)
  {
      if (CheckCollisionRecs(bounds, wall))
      {
          bounds.x = previousX;
          break;
      }
  }

  // Try vertical movement.
  const float previousY = bounds.y;

  bounds.y += direction.y * speed * deltaTime;

  for (const Rectangle& wall : walls) //loops through vector of walls
  {
      if (CheckCollisionRecs(bounds, wall)) //returns true or false based on rectangle overrlap
      {
          bounds.y = previousY; //if true cant move further
          break;
      }
  }

  bounds.x = std::clamp( //x and y clamp to gamescreen
        bounds.x,
        0.0f,
        static_cast<float>(GetScreenWidth()) - bounds.width
    );

    bounds.y = std::clamp(
        bounds.y,
        0.0f,
        static_cast<float>(GetScreenHeight()) - bounds.height
    );
  


}

void Player::Draw() const
{
  DrawRectangleRec(bounds, SKYBLUE);
}