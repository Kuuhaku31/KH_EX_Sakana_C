
// player.cpp

#include "bullet_time_manager.h"
#include "player.h"

void
Player::on_input(const ExMessage& msg)
{
    // hp<=0时，不接受输入
    if(hp <= 0) return;

    // 键位映射
    switch(msg.message)
    {
        // 按键按下
    case WM_KEYDOWN:
        switch(msg.vkcode)
        {
        case 0x41: // A
            is_left_key_down = true;
            break;

        case 0x44: // D
            is_right_key_down = true;
            break;

        case 0x57: // W
        case VK_SPACE:
            is_jump_key_down = true;
            break;

        case 0x53: // S
            is_roll_key_down = true;
            break;

        case VK_SHIFT: // 翻滚
            // 进入子弹时间
            play_audio(_T("bullet_time"), false);
            BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Entering);
            break;

        case VK_UP: // 向上攻击
            is_attack_key_down = true;
            attack_dir         = AttackDir::Up;
            break;

        case VK_DOWN: // 向下攻击
            is_attack_key_down = true;
            attack_dir         = AttackDir::Down;
            break;

        case VK_LEFT: // 向左攻击
            is_attack_key_down = true;
            attack_dir         = AttackDir::Left;
            break;

        case VK_RIGHT: // 向右攻击
            is_attack_key_down = true;
            attack_dir         = AttackDir::Right;
            break;
        }
        break;

    case WM_KEYUP:
        switch(msg.vkcode)
        {
        case 0x41: // A
            is_left_key_down = false;
            break;

        case 0x44: // D
            is_right_key_down = false;
            break;

        case 0x57: // W
        case VK_SPACE:
            is_jump_key_down = false;
            break;

        case 0x53: // S
            is_roll_key_down = false;
            break;

        case VK_SHIFT: // 翻滚
            // 退出子弹时间
            play_audio(_T("bullet_time"), false);
            BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Exiting);
            break;

        case VK_UP:    // 向上攻击
        case VK_DOWN:  // 向下攻击
        case VK_LEFT:  // 向左攻击
        case VK_RIGHT: // 向右攻击
            is_attack_key_down = false;
            break;
        }
        break;

        // 鼠标按下...
    }
}

void
Player::on_update(float delta)
{
    // 更新速度，如果处于翻滚、被击退状态，则不更新速度
    if(hp > 0 && !is_rolling && !is_repulsed)
    {
        velocity.vx = get_move_axis() * SPEED_RUN;
    }

    // 更新朝向
    if(get_move_axis())
    {
        is_facing_left = (get_move_axis() < 0);
    }

    // 更新计时器
    timer_roll_cd.on_update(delta);
    timer_attack_cd.on_update(delta);
    animation_jump_vfx.on_update(delta);
    animation_land_vfx.on_update(delta);
    if(is_attacking)
    {
        current_slash_animation->set_position(get_logic_center());
        current_slash_animation->on_update(delta);
    }

    // 基类更新
    Character::on_update(delta);
}

void
Player::on_render()
{
    // 渲染跳跃特效
    if(is_jump_vfx_visible)
    {
        animation_jump_vfx.on_render();
    }
    if(is_land_vfx_visible)
    {
        animation_land_vfx.on_render();
    }

    // 渲染角色
    Character::on_render();

    // 渲染攻击特效
    if(is_attacking)
    {
        current_slash_animation->on_render();
    }
}

void
Player::on_hurt()
{
    play_audio(_T("player_hurt"), false);
}

void
Player::on_jump()
{
    velocity.vy -= SPEED_JUMP;
    is_jump_vfx_visible = true;
    animation_jump_vfx.set_position(position);
    animation_jump_vfx.reset();
}

// 角色落地
void
Player::on_land()
{
    is_land_vfx_visible = true;
    animation_land_vfx.set_position(position);
    animation_land_vfx.reset();
}

void
Player::on_roll()
{
    timer_roll_cd.restart();
    is_roll_cd_comp = false;
    velocity.vx     = is_facing_left ? -SPEED_ROLL : SPEED_ROLL;
}

void
Player::on_attack()
{
    timer_attack_cd.restart();
    is_attack_cd_comp = false;

    switch(attack_dir)
    {
    case AttackDir::Up:
        current_slash_animation = &animation_slash_up;
        break;

    case AttackDir::Down:
        current_slash_animation = &animation_slash_down;
        break;

    case AttackDir::Left:
        current_slash_animation = &animation_slash_left;
        break;

    case AttackDir::Right:
        current_slash_animation = &animation_slash_right;
        break;
    }

    current_slash_animation->set_position(get_logic_center());
    current_slash_animation->reset();
}

void
Player::update_attack_dir(int x, int y)
{
    static const float PI = 3.1415926f;

    // 计算角度
    float angle = atan2(y - position.vy, x - position.vx) * 180 / PI;

    // 更新攻击方向
    if(angle >= -45 && angle <= 45)
    {
        attack_dir = AttackDir::Right;
    }
    else if(angle > 45 && angle <= 135)
    {
        attack_dir = AttackDir::Down;
    }
    else if(angle > 135 || angle <= -135)
    {
        attack_dir = AttackDir::Left;
    }
    else
    {
        attack_dir = AttackDir::Up;
    }
}
