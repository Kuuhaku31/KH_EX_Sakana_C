
// character.h

#pragma once

#include "animation.h"
#include "collision_manager.h"
#include "state_machine.h"

class Character
{
public:
    Character();
    ~Character();

    int            get_hp() const { return hp; }
    void           set_position(const Vector2& pos) { position = pos; }
    const Vector2& get_position() const { return position; }
    void           set_velocity(const Vector2& vel) { velocity = vel; }
    const Vector2& get_velocity() const { return velocity; }
    Vector2        get_logic_center() const { return Vector2{ position.vx, position.vy - logic_height / 2 }; }
    void           set_gravity_enable(bool flag) { enable_gravity = flag; }
    CollisionBox*  get_hit_box() const { return hit_box; }
    CollisionBox*  get_hurt_box() const { return hurt_box; }
    bool           is_on_floor() const { return position.vy >= FLOOR_Y; }
    float          get_floor_y() const { return FLOOR_Y; }

    void
    make_invulnerable()
    {
        is_invulnerable = true;
        timer_invulnerable_status.restart();
    }

    void decrease_hp(); // 减少生命值

    virtual void on_input(const ExMessage& msg); // 输入处理
    virtual void on_update(float delta);         // 更新
    virtual void on_render();                    // 渲染
    virtual void on_hurt();                      // 受击

    void switch_state(const std::string& id);  // 切换状态
    void set_animation(const std::string& id); // 设置动画

protected:
    // 角色动画组
    struct AnimationGroup
    {
        Animation ani_L;
        Animation ani_R;
    };

protected:
    const float FLOOR_Y = 620;     // 地板的竖直方向坐标
    const float GRAVITY = 980 * 2; // 重力大小

protected:
    int     hp = 10;                    // 角色生命值
    Vector2 position;                   // 角色脚底位置
    Vector2 velocity;                   // 角色速度
    float   logic_height       = 0;     // 角色的逻辑高度
    bool    is_facing_left     = true;  // 当前角色是否朝向左
    bool    enable_gravity     = true;  // 启用重力模拟
    bool    is_invulnerable    = false; // 当前是否无敌
    bool    is_blink_invisible = false; // 当前是否处于闪烁的不可见帧
    Timer   timer_invulnerable_status;  // 无敌状态定时器
    Timer   timer_invulnerable_blink;   // 无敌闪烁状态定时器

    StateMachine    state_machine;               // 角色逻辑状态机
    CollisionBox*   hit_box           = nullptr; // 攻击碰撞箱
    CollisionBox*   hurt_box          = nullptr; // 受击碰撞箱
    AnimationGroup* current_animation = nullptr; // 当前角色动画

    std ::unordered_map<std ::string, AnimationGroup> animation_pool; // 角色动画池
};
