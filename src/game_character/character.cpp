
// character.cpp

#include "character.h"
#include "collision_manager.h"

Character::Character()
{
    // 初始化碰撞盒
    hit_box  = CollisionManager::instance()->create_collision_box();
    hurt_box = CollisionManager::instance()->create_collision_box();

    // 初始化计时器
    // 无敌状态定时器
    timer_invulnerable_status.set_wait_time(0.8f);
    timer_invulnerable_status.set_one_shot(true);
    timer_invulnerable_status.set_on_timeout([&]() {
        is_invulnerable = false;
        is_blink        = false;

        hit_box->set_enable(true);
    });

    // 无敌闪烁状态定时器
    timer_invulnerable_blink.set_wait_time(0.075f);
    timer_invulnerable_blink.set_one_shot(false);
    timer_invulnerable_blink.set_on_timeout([&]() {
        is_blink_invisible = !is_blink_invisible;
    });
}

Character::~Character()
{
    // 销毁碰撞盒
    CollisionManager::instance()->destroy_collision_box(hit_box);
}

void
Character::decrease_hp()
{
    if(is_invulnerable)
    {
        return;
    }

    hp--;

    make_invulnerable();
    on_hurt();
}

void
Character::on_input(const ExMessage& msg)
{
}

void
Character::on_update(float delta)
{
    // 首先更新状态机
    state_machine.on_update(delta);

    // 如果启用重力模拟
    if(enable_gravity)
    {
        velocity.vy += GRAVITY * delta;
    }

    // 更新位置
    position += velocity * delta;

    // 如果角色在地面下方
    if(position.vy >= FLOOR_Y)
    {
        // 修正位置和速度
        position.vy = FLOOR_Y;
        velocity.vy = 0;
    }

    // 限制角色水平位置
    if(position.vx <= 0) position.vx = 0;
    if(position.vx >= getwidth()) position.vx = (float)getwidth();

    // 更新碰撞盒位置
    hurt_box->set_position(get_logic_center());

    // 更新无敌状态定时器
    timer_invulnerable_status.on_update(delta);

    // 更新无敌闪烁状态定时器
    if(is_invulnerable)
    {
        timer_invulnerable_blink.on_update(delta);
    }

    // 更新动画
    if(current_animation)
    {
        Animation& animation = (is_facing_left ? current_animation->ani_L : current_animation->ani_R);
        animation.on_update(delta);
        animation.set_position(position);
    }
}

void
Character::on_render()
{
    // 如果角色无敌且处于闪烁的不可见帧
    if((is_invulnerable && is_blink_invisible && is_blink))
    {
        return;
    }

    // 渲染动画
    if(current_animation)
    {
        Animation& animation = (is_facing_left ? current_animation->ani_L : current_animation->ani_R);
        animation.on_render();
    }
}

void
Character::on_hurt()
{
}

void
Character::switch_state(const std::string& id)
{
    state_machine.switch_to(id);
}

void
Character::set_animation(const std::string& id)
{
    // 从动画池中获取动画
    current_animation = &animation_pool[id];

    // 重置动画
    current_animation->ani_L.reset();
    current_animation->ani_R.reset();
}
