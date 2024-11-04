
// repulsed.cpp

#include "enemy_state_nodes.h"

#define SPEED_REPULSED 300.0f

void
EnemyRepulsedState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    // 设置敌人的动画为下蹲
    enemy->set_animation("squat");
    enemy->set_velocity(Vector2{ enemy->get_facing_left() ? SPEED_REPULSED : -SPEED_REPULSED, 0 });
    enemy->set_repulsed(true);
}

void
EnemyRepulsedState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    // 空气阻力
    const float AIR_RESISTANCE = 10;

    // 减速
    Vector2 v  = enemy->get_velocity();
    float   vt = v.vx > 0 ? v.vx - AIR_RESISTANCE : v.vx + AIR_RESISTANCE;

    if(enemy->get_hp() <= 0) enemy->switch_state("dead");

    // 如果速度的方向发生了变化，直接设置速度为0，并切换到闲置状态
    if(v.vx * vt <= 0)
    {
        enemy->set_velocity(Vector2{ 0, 0 });
        enemy->set_repulsed(false);
        enemy->switch_state("idle");
        return;
    }

    v.vx = vt;
    enemy->set_velocity(v);
}
