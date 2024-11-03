
// sword.cpp

#include "sword.h"

Sword::Sword(const Vector2& pos, bool move_left)
{
    // 初始化动画
    animation.set_interval(0.1f);
    animation.set_loop(true);
    animation.set_anchor_mode(Animation::AnchorMode::Centered);
    animation.add_frame(ResourcesManager::instance()->find_atlas(move_left ? "sword_left" : "sword_right"));

    // 初始碰撞盒
    collision_box = CollisionManager::instance()->create_collision_box();
    collision_box->set_layer_src(CollisionLayer::Enemy);
    collision_box->set_layer_dst(CollisionLayer::Player);
    collision_box->set_size(Vector2{ 195, 10 });
    collision_box->set_on_collide([&]() {
        // 当剑被击中时的回调函数
        is_valid = false;
    });

    position = pos;
    velocity = Vector2{ move_left ? -SPEED_MOVE : SPEED_MOVE, 0 };
}

Sword::~Sword()
{
    // 销毁碰撞盒
    CollisionManager::instance()->destroy_collision_box(collision_box);
}

void
Sword::on_update(float delta)
{
    // 更新位置
    position += velocity * delta;
    animation.set_position(position);
    collision_box->set_position(position);

    // 更新动画
    animation.on_update(delta);

    // 当剑超出屏幕范围时，标记为无效
    if(position.vx <= -200 || position.vx >= getwidth() + 200) is_valid = false;
}

void
Sword::on_render()
{
    animation.on_render();
}
