
// idle //

#include "player_state_nodes.h"

PlayerIdleState::PlayerIdleState()  = default;
PlayerIdleState::~PlayerIdleState() = default;

void
PlayerIdleState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("idle");
}

void
PlayerIdleState::on_update(float delta)
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if(player->can_attack())
    {
        player->switch_state("attack");
    }
    else if(player->get_velocity().vy > 0)
    {
        player->switch_state("fall");
    }
    else if(player->can_jump())
    {
        player->switch_state("jump");
    }
    else if(player->can_roll())
    {
        player->switch_state("roll");
    }
    else if(player->get_move_axis() != 0 && player->is_on_floor())
    {
        player->switch_state("run");
    }
}
