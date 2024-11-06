
// repulsed.cpp

#include "enemy_state_nodes.h"

#define SPEED_REPULSED 300.0f

void
EnemyRepulsedState::on_enter()
{
    Enemy*  enemy  = (Enemy*)CharacterManager::instance()->get_enemy();
    Player* player = (Player*)CharacterManager::instance()->get_player();

    enemy->set_repulsed(true);
    enemy->set_hit_box_enable(false);
    enemy->set_facing_left(player->get_position());
    enemy->set_animation("repulsed");
    enemy->set_velocity_x(enemy->get_facing_left() ? SPEED_REPULSED : -SPEED_REPULSED);

    play_audio(_T("colliding"), false);
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

    if(enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
    else if(v.vx * vt <= 0)
    {
        // 如果速度的方向发生了变化，直接设置速度为0，并切换到闲置状态
        enemy->set_velocity_x(0);

        if(enemy->is_on_floor())
        {
            enemy->switch_state("idle");
        }
        else
        {
            enemy->switch_state("fall");
        }
    }
    else
    {
        v.vx = vt;
        enemy->set_velocity(v);
    }
}

void
EnemyRepulsedState::on_exit()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    enemy->set_hit_box_enable(true);
    enemy->set_repulsed(false);
}
