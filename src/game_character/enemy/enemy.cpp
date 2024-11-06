
// enemy.cpp

#include "bullet_time_manager.h"
#include "enemy.h"

#include <algorithm>

static bool
fun_del_barb(Barb* barb)
{
    bool can_remove = !barb->check_valid();
    if(can_remove) delete barb;
    return can_remove;
}

static bool
fun_del_sword(Sword* sword)
{
    bool can_remove = !sword->check_valid();
    if(can_remove) delete sword;
    return can_remove;
}

void
Enemy::on_update(float delta)
{
    // 调用父类的更新逻辑
    Character::on_update(delta);

    // 更新碰撞盒逻辑
    hit_box->set_position(get_logic_center());

    // 更新技能逻辑
    on_update_skills(delta);
}

void
Enemy::on_update_skills(float delta)
{
    if(is_throwing_silk)
    {
        collision_box_silk->set_position(get_logic_center());
        animation_silk.set_position(get_logic_center());
        animation_silk.on_update(delta);
    }
    if(is_dashing_in_air || is_dashing_on_floor)
    {
        current_dash_animation->set_position(get_logic_center());
        current_dash_animation->on_update(delta);
    }
    for(Barb* barb : barb_list) barb->on_update(delta);
    for(Sword* sword : sword_list) sword->on_update(delta);

    // 移除已经失效的技能
    barb_list.erase(std::remove_if(barb_list.begin(), barb_list.end(), fun_del_barb), barb_list.end());
    sword_list.erase(std::remove_if(sword_list.begin(), sword_list.end(), fun_del_sword), sword_list.end());
}

void
Enemy::on_render()
{
    // 渲染技能
    for(Barb* barb : barb_list) barb->on_render();
    for(Sword* sword : sword_list) sword->on_render();

    // 调用父类的渲染逻辑
    Character::on_render();

    // 渲染特效
    if(is_throwing_silk) animation_silk.on_render();
    if(is_dashing_in_air || is_dashing_on_floor) current_dash_animation->on_render();
}

void
Enemy::on_hurt()
{
    if(is_invulnerable) return;

    BulletTimeManager::instance()->add_energy(0.5f); // 击中敌人时增加子弹时间能量

    if(is_dashing_on_floor || is_dashing_in_air)
    {
        switch_state("repulsed");
        make_invulnerable(0.5f, false);
    }
    else
    {
        hp--;

        switch(range_random(1, 3))
        {
        case 1: play_audio(_T("enemy_hurt_1"), false); break;
        case 2: play_audio(_T("enemy_hurt_2"), false); break;
        case 3: play_audio(_T("enemy_hurt_3"), false); break;
        }

        // 进入击退状态
        switch_state("repulsed");
        make_invulnerable(1.5f, true);
    }
}

void
Enemy::shoot_barb(const Vector2& target)
{
    Barb* barb = new Barb();
    barb->set_position(get_logic_center());
    barb->on_shoot(target); // 直接发射向玩家
    barb_list.push_back(barb);
}

void
Enemy::throw_barbs()
{
    int num_new_barbs = range_random(3, 6);       // 随机生成3-6个飞刺
    if(barb_list.size() >= 10) num_new_barbs = 1; // 飞刺数量达到上限时，只生成一个飞刺
    int wide_grid = getwidth() / num_new_barbs;   // 将屏幕宽度等分为num_new_barbs个网格

    // 生成飞刺
    for(int i = 0; i < num_new_barbs; i++)
    {
        Barb* barb = new Barb();

        int rand_x = range_random(wide_grid * i, wide_grid * (i + 1)); // 在网格内随机生成x坐标
        int rand_y = range_random(250, 500);                           // 在y轴上随机生成y坐标

        barb->set_position(Vector2{ (float)rand_x, (float)rand_y });
        barb_list.push_back(barb);
    }
}

void
Enemy::throw_swords()
{
    Sword* sword = new Sword(get_logic_center(), is_facing_left);
    sword_list.push_back(sword);
}

void
Enemy::on_dash()
{
    if(is_dashing_in_air)
    {
        current_dash_animation = velocity.vx < 0 ? &animation_dash_in_air_vfx.ani_L : &animation_dash_in_air_vfx.ani_R;
    }
    else
    {
        current_dash_animation = velocity.vx < 0 ? &animation_dash_on_floor_vfx.ani_L : &animation_dash_on_floor_vfx.ani_R;
    }

    current_dash_animation->reset();
}

void
Enemy::on_throw_silk()
{
    animation_silk.reset();
}
