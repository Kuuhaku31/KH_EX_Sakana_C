
// collision_box.h

#pragma once

#include "vector2.h"
#include <functional>

enum class CollisionLayer
{
    None         = 0,
    Player       = 1,
    Enemy        = 2,
    PlayerBullet = 4,
    EnemyBullet  = 8,
    PlayerVfx    = 16,
    EnemyVfx     = 32,
    All          = 0xFFFF,
};

class CollisionBox
{
    friend class CollisionManager;

public:
    // 设置碰撞盒是否启用
    void
    set_enable(bool flag)
    {
        enable = flag;
    }

    // 设置碰撞盒大小
    void
    set_size(const Vector2& s)
    {
        size = s;
    }

    // 设置碰撞盒位置
    void
    set_position(const Vector2& pos)
    {
        position = pos;
    }

    // 设置源碰撞层
    void
    set_layer_src(CollisionLayer layer)
    {
        layer_src = layer;
    }

    // 设置目标碰撞层
    void
    set_layer_dst(CollisionLayer layer)
    {
        layer_dst = layer;
    }

    // 设置回调函数
    void
    set_on_collide(std::function<void()> f)
    {
        on_collision = f;
    }

    // 获取碰撞盒大小
    const Vector2&
    get_size() const
    {
        return size;
    }

private:
    Vector2 size;          // 碰撞盒大小
    Vector2 position;      // 碰撞盒位置
    bool    enable = true; // 是否启用碰撞盒

    std::function<void()> on_collision; // 碰撞回调函数

    CollisionLayer layer_src = CollisionLayer::None; // 源碰撞层
    CollisionLayer layer_dst = CollisionLayer::None; // 目标碰撞层

private:
    CollisionBox()  = default;
    ~CollisionBox() = default;
};
