#include "Player.h"

#include <algorithm>
#include <cmath>

Player::Player(float x, float y)
  : bounds{x, y, 32.0, 32.0}
{

}

void Player::Update(float deltaTime, const std::vector<Rectangle>& walls)
{

  damageCooldown = std::max(0.0f, damageCooldown - deltaTime);

  damageFlashTimer = std::max(
    0.0f,
    damageFlashTimer - deltaTime
  );

  if (IsDead())
  {
    return;
  }

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
  
  const Vector2 playerCenter{
    bounds.x + bounds.width / 2.0f,
    bounds.y + bounds.height / 2.0f
  };

  const Vector2 mouse = GetMousePosition();

  Vector2 mouseDirection{
      mouse.x - playerCenter.x,
      mouse.y - playerCenter.y
  };

  const float aimLength = std::sqrt(
      mouseDirection.x * mouseDirection.x +
      mouseDirection.y * mouseDirection.y
  );

  if (aimLength > 0.001f)
  {
      aimDirection.x = mouseDirection.x / aimLength;
      aimDirection.y = mouseDirection.y / aimLength;
  }
  
}

void Player::Draw() const
{
    Color color = SKYBLUE;

    if (damageFlashTimer > 0.0f)
    {
        constexpr float flashDuration = 0.2f;
        constexpr float flashInterval = 0.05f;

        const float elapsed = flashDuration - damageFlashTimer;
        const int phase = static_cast<int>(elapsed / flashInterval);

        color = (phase % 2 == 0) ? RED : WHITE;
    }

    DrawRectangleRec(bounds, color);

    const Vector2 center{
    bounds.x + bounds.width / 2.0f,
    bounds.y + bounds.height / 2.0f
  };

  constexpr float orbitRadius = 30.0f;
  constexpr float triangleLength = 12.0f;
  constexpr float triangleHalfWidth = 6.0f;

  // Position the triangle on a circle around the player.
  const Vector2 markerCenter{
      center.x + aimDirection.x * orbitRadius,
      center.y + aimDirection.y * orbitRadius
  };

  // A direction perpendicular to the aim direction.
  const Vector2 sideways{
      -aimDirection.y,
      aimDirection.x
  };

  // Point faces away from the player.
  const Vector2 tip{
      markerCenter.x + aimDirection.x * triangleLength / 2.0f,
      markerCenter.y + aimDirection.y * triangleLength / 2.0f
  };

  const Vector2 baseCenter{
      markerCenter.x - aimDirection.x * triangleLength / 2.0f,
      markerCenter.y - aimDirection.y * triangleLength / 2.0f
  };

  const Vector2 baseLeft{
      baseCenter.x - sideways.x * triangleHalfWidth,
      baseCenter.y - sideways.y * triangleHalfWidth
  };

  const Vector2 baseRight{
      baseCenter.x + sideways.x * triangleHalfWidth,
      baseCenter.y + sideways.y * triangleHalfWidth
  };

  DrawTriangle(tip, baseLeft, baseRight, GOLD);
}

Rectangle Player::GetBounds() const {

  return bounds;
}

void Player::SetPosition(Vector2 position) {

  bounds.x = position.x;
  bounds.y = position.y;
}

void Player::TakeDamage(int damage) { //method for player to take damage

  if (damage <= 0 || damageCooldown > 0.0f || IsDead())
  {
      return;
  }

  health = std::max(0, health - damage);
  damageCooldown = 1.0f;
  damageFlashTimer = 0.2f;
}

int Player::GetHealth() const { //returns current player health

  return health;
}

bool Player::IsDead() const { //returns bool if player has no health remaining

  return health == 0;
}