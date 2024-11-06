
// repulsed.cpp

#include "player_state_nodes.h"

#define SPEED_REPULSED 500.0f

void
PlayerRepulsedState::on_enter()
{
    Player* player = (Player*)CharacterManager::instance()->get_player();
    Enemy*  enemy  = (Enemy*)CharacterManager::instance()->get_enemy();

    player->set_repulsed(true);
    player->set_facing_left(enemy->get_position());
    player->set_animation("repulsed");
    player->set_velocity_x(player->get_facing_left() ? SPEED_REPULSED : -SPEED_REPULSED);
}

void
PlayerRepulsedState::on_update(float delta)
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 空气阻力
    const float AIR_RESISTANCE = 10;

    // 减速
    Vector2 v  = player->get_velocity();
    float   vt = v.vx > 0 ? v.vx - AIR_RESISTANCE : v.vx + AIR_RESISTANCE;

    if(player->get_hp() <= 0)
    {
        player->set_repulsed(false);
        player->switch_state("dead");
    }
    else if(v.vx * vt <= 0)
    {
        // 如果速度的方向发生了变化，直接设置速度为0，并切换到闲置状态
        player->set_velocity_x(0);
        player->set_repulsed(false);
        player->switch_state("idle");
    }
    else
    {
        player->set_velocity_x(vt);
    }
}
