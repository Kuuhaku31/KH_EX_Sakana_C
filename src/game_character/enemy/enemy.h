
// enemy.h

#pragma once

#include "barb.h"
#include "character.h"
#include "sword.h"

class Enemy : public Character
{
public:
    Enemy();
    ~Enemy();

    void on_update(float delta) override;
    void on_render() override;

    void on_hurt() override;

    void on_update_skills(float delta); // 更新技能逻辑

    void shoot_barb(const Vector2& target);
    void throw_barbs();
    void throw_swords();

    void on_dash();
    void on_throw_silk();

    // 获取、设置自身状态 //

    void set_throwing_silk(bool flag)
    {
        is_throwing_silk = flag;
        collision_box_silk->set_enable(flag);
    }
    bool get_throwing_silk() const { return is_throwing_silk; }

    void set_dash_in_air(bool flag) { is_dashing_in_air = flag; }
    bool get_dash_in_air() const { return is_dashing_in_air; }

    void set_dash_on_floor(bool flag) { is_dashing_on_floor = flag; }
    bool get_dash_on_floor() const { return is_dashing_on_floor; }

    void set_hit_box_enable(bool flag) { hit_box->set_enable(flag); }

private:
    bool is_throwing_silk    = false; // 是否正使用投射技能
    bool is_dashing_in_air   = false; // 是否正使用空中冲刺技能
    bool is_dashing_on_floor = false; // 是否正使用地面冲刺技能

    Animation      animation_silk;
    AnimationGroup animation_dash_in_air_vfx;
    AnimationGroup animation_dash_on_floor_vfx;
    Animation*     current_dash_animation = nullptr;

    std::vector<Barb*>  barb_list;
    std::vector<Sword*> sword_list;
    CollisionBox*       collision_box_silk = nullptr;
};
