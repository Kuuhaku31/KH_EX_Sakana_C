
// repulsed.cpp

#include "player_state_nodes.h"

#define SPEED_REPULSED 300.0f

void
PlayerRepulsedState::on_enter()
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 设置玩家的动画
    player->set_animation("repulsed");
    player->set_velocity(Vector2{ player->get_facing_left() ? SPEED_REPULSED : -SPEED_REPULSED, 0 });
    player->set_repulsed(true);
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
        player->set_velocity(Vector2{ 0, 0 });
        player->set_repulsed(false);
        player->switch_state("idle");
    }
    else
    {
        v.vx = vt;
        player->set_velocity(v);
    }
}
