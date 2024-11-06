
// barb.cpp

#include "barb.h"
#include "bullet_time_manager.h"
#include "character_manager.h"
#include "collision_manager.h"
#include "resources_manager.h"

Barb::Barb()
{
    // 随机决定初始偏移
    diff_period = range_random(0, 6);

    // 初始化闲置动画
    animation_loose.set_interval(0.15f);
    animation_loose.set_loop(true);
    animation_loose.set_anchor_mode(Animation::AnchorMode::Centered);
    animation_loose.add_frame(ResourcesManager::instance()->find_atlas("barb_loose"));

    // 初始化破碎动画
    animation_break.set_interval(0.1f);
    animation_break.set_loop(false);
    animation_break.set_anchor_mode(Animation::AnchorMode::Centered);
    animation_break.add_frame(ResourcesManager::instance()->find_atlas("barb_break"));
    animation_break.set_on_finished([&]() { is_valid = false; });

    // 初始化碰撞盒
    collision_box = CollisionManager::instance()->create_collision_box();
    collision_box->set_layer_src(CollisionLayer::Enemy);
    collision_box->set_layer_dst(CollisionLayer::Player);
    collision_box->set_size({ 20, 20 });
    collision_box->set_on_collide([&]() {
        on_break();
        BulletTimeManager::instance()->add_energy(0.1f);
    });

    // 初始化闲置计时器
    timer_idle.set_wait_time((float)range_random(3, 10));
    timer_idle.set_one_shot(true);
    timer_idle.set_on_timeout([&]() {
        if(stage == Stage::Idle)
        {
            stage         = Stage::Aim;
            base_position = current_position;
        }
    });

    // 初始化瞄准计时器
    timer_aim.set_wait_time(0.75f);
    timer_aim.set_one_shot(true);
    timer_aim.set_on_timeout([&]() {
        if(stage == Stage::Aim)
        {
            Player* player = (Player*)CharacterManager::instance()->get_player();
            Vector2 target = player->get_logic_center();
            target.vy -= 30;
            on_shoot(target);
        }
    });
}

Barb::~Barb()
{
    CollisionManager::instance()->destroy_collision_box(collision_box);
}

void
Barb::on_update(float delta)
{
    // 更新定时器逻辑
    if(stage == Stage::Idle)
        timer_idle.on_update(delta);
    if(stage == Stage::Aim)
        timer_aim.on_update(delta);

    // 更新移动逻辑
    total_delta_time += delta;
    switch(stage)
    {
    case Barb::Stage::Idle:
        // 利用正弦余弦函数实现刺球的上下浮动
        current_position.vy = base_position.vy + sin(total_delta_time * 2 + diff_period) * 30;
        break;
    case Barb::Stage::Aim:
        current_position.vx = base_position.vx + range_random(-10, 10);
        break;
    case Barb::Stage::Dash:
        current_position += velocity * delta;
        if(current_position.vy >= CharacterManager::instance()->get_player()->get_floor_y())
            on_break();
        if(current_position.vy <= 0)
            is_valid = false;
        break;
    }

    collision_box->set_position(current_position);

    // 更新动画逻辑
    current_animation = (stage == Stage::Break ? &animation_break : &animation_loose);
    current_animation->set_position(current_position);
    current_animation->on_update(delta);
}

void
Barb::on_render()
{
    current_animation->on_render();
}

// 向玩家发射
void
Barb::on_shoot(const Vector2& target)
{
    // 切换状态，设置速度
    stage = Stage::Dash;

    velocity = (target - current_position).tounit() * SPEED_DASH;
}

void
Barb::on_break()
{
    if(stage == Stage::Break) return;
    stage = Stage::Break;

    collision_box->set_enable(false);    // 禁用碰撞盒
    play_audio(_T("barb_break"), false); // 播放音效
}
