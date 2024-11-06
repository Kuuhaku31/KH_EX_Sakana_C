
// sword.h

#pragma once

#include "animation.h"
#include "collision_box.h"

class Sword
{
public:
    Sword(const Vector2& position, bool move_left);
    ~Sword();

    void on_update(float delta);
    void on_render();

    bool check_valid() const { return is_valid; }

private:
    const float SPEED_MOVE = 2000.0f;

private:
    Vector2   position;        // 位置
    Vector2   velocity;        // 速度
    Animation animation;       // 动画
    bool      is_valid = true; // 是否有效

    // 碰撞盒
    CollisionBox* collision_box = nullptr;
};
