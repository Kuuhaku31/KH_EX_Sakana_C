
// jump //

#include "character_manager.h"
#include "player_state_nodes.h"

PlayerJumpState::PlayerJumpState()  = default;
PlayerJumpState::~PlayerJumpState() = default;

void
PlayerJumpState::on_enter()
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    player->set_animation("jump");
    player->on_jump();

    play_audio(_T("player_jump"), false);
}

void
PlayerJumpState::on_update(float delta)
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if(player->get_velocity().vy > 0)
    {
        player->switch_state("fall");
    }
    else if(player->can_attack())
    {
        player->switch_state("attack");
    }
}
