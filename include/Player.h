#pragma once

#include "raylib.h"
#include <vector>

struct AttackZone
{
    Vector2 center;
    Vector2 direction;
    float length;
    float width;
};

class Player {
  public:  //contains methods other classes can call
    Player(float x, float y);

    void Update(float deltaTime, const std::vector<Rectangle>& walls);
    void Draw() const;
    Rectangle GetBounds() const;
    void SetPosition(Vector2 position);

    void TakeDamage(int damage);
    int GetHealth() const;
    bool IsDead() const;  

    bool DidAttackThisFrame() const;
    bool AttackHits(Rectangle target) const;
    void CancelAttack();
      bool HasClearAttackPath(
      Rectangle target,
      const std::vector<Rectangle>& walls
    ) const;



  private: //priate refers to data managed by the class
    Rectangle bounds;
    float speed = 220.0f;
    int health = 100;
    float damageCooldown = 0.0f;
    float damageFlashTimer = 0.0f;
    Vector2 aimDirection{1.0f, 0.0f};

    AttackZone attackZone{};
    bool attackedThisFrame = false;

    float attackTimer = 0.0f;
    float attackCooldown = 0.0f;

    static constexpr float attackDuration = 0.15f;
    static constexpr float attackDelay = 0.35f;
};