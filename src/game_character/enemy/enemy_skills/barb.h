
// barb.h

#pragma once

#include "animation.h"
#include "collision_box.h"

class Barb
{
public:
    Barb();
    ~Barb();

    void on_update(float delta);
    void on_render();

    void set_position(const Vector2& pos) { base_position = current_position = pos; }
    bool check_valid() const { return is_valid; }

private:
    //  刺球的四种状态
    enum class Stage
    {
        Idle, // 默认状态
        Aim,  // 瞄准
        Dash, // 冲刺
        Break // 破碎
    };

private:
    const float SPEED_DASH = 1500.0f;

private:
    Timer timer_idle;              // 空闲状态计时器
    Timer timer_aim;               // 瞄准状态计时器
    int   diff_period      = 0;    // 随机数，用于控制浮动周期偏移
    float total_delta_time = 0;    // 刺球存在时间
    bool  is_valid         = true; // 是否有效

    Vector2 velocity;         // 速度
    Vector2 base_position;    // 初始位置
    Vector2 current_position; // 当前位置

    Animation  animation_loose;
    Animation  animation_break;
    Animation* current_animation = nullptr;

    Stage         stage         = Stage ::Idle;
    CollisionBox* collision_box = nullptr;

private:
    // 处理刺球破碎逻辑
    void on_break();
};
