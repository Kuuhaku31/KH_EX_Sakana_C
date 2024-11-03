
// palyer.h

#pragma once

#include "character.h"

class Player : public Character
{
public:
    enum class AttackDir
    {
        Up,
        Down,
        Left,
        Right
    };

public:
    Player();
    ~Player();

    void on_input(const ExMessage& msg) override;
    void on_update(float delta) override;
    void on_render() override;
    void on_hurt() override;

public:
    // 角色翻滚状态接口

    void set_rolling(bool flag) { is_rolling = flag; }
    bool get_rolling() const { return is_rolling; }
    bool can_roll() const { return is_roll_cd_comp && !is_rolling && is_roll_key_down; } // 滚动CD完成,且未滚动,且按下滚动键


    // 角色攻击状态接口

    void set_attacking(bool flag) { is_attacking = flag; }
    bool get_attacking() const { return is_attacking; }
    bool can_attack() const { return is_attack_cd_comp && !is_attacking && is_attack_key_down; } // 攻击CD完成,且未攻击,且按下攻击键

    bool      can_jump() const { return is_on_floor() && is_jump_key_down; }         // 角色能否跳跃
    int       get_move_axis() const { return is_right_key_down - is_left_key_down; } // 获取角色移动方向
    AttackDir get_attack_dir() const { return attack_dir; }                          // 角色攻击方向

public:
    void on_jump();   // 需要在角色跳跃时调用
    void on_land();   // 需要在角色落地时调用
    void on_roll();   // 需要在角色翻滚时调用
    void on_attack(); // 需要在角色攻击时调用

private:
    // 传入鼠标坐标，更新角色攻击方向
    void update_attack_dir(int x, int y);

private:
    // 角色翻滚
    Timer timer_roll_cd;
    bool  is_rolling      = false;
    bool  is_roll_cd_comp = true;

    // 角色攻击
    Timer timer_attack_cd;
    bool  is_attacking      = false;
    bool  is_attack_cd_comp = true;

    // 键位映射
    bool is_left_key_down   = false;
    bool is_right_key_down  = false;
    bool is_jump_key_down   = false;
    bool is_roll_key_down   = false;
    bool is_attack_key_down = false;

    // 角色攻击动画
    Animation  animation_slash_up;
    Animation  animation_slash_down;
    Animation  animation_slash_left;
    Animation  animation_slash_right;
    AttackDir  attack_dir              = AttackDir ::Right;
    Animation* current_slash_animation = nullptr;

    // 角色跳跃特效
    bool      is_jump_vfx_visible = false; // 角色跳跃特效是否可见
    Animation animation_jump_vfx;

    // 角色落地特效
    bool      is_land_vfx_visible = false;
    Animation animation_land_vfx;

private:
    const float CD_ROLL    = 0.75f;
    const float CD_ATTACK  = 0.5f;
    const float SPEED_RUN  = 300.0f;
    const float SPEED_JUMP = 780.0f;
    const float SPEED_ROLL = 600.0f;
};
