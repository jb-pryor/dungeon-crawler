#include "Player.h"

#include <algorithm>
#include <cmath>

Player::Player(float x, float y)
  : bounds(x, y, 32.0, 32.0)
{

}

void Player::Update(float deltaTime)
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
    direction.y /= length;
  }

  bounds.x += direction.x * speed * deltaTime;
  bounds.y += direction.y * speed * deltaTime;

  bounds.x = std::clamp(
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