
// fall //

#include "character_manager.h"
#include "player_state_nodes.h"

PlayerFallState::PlayerFallState()  = default;
PlayerFallState::~PlayerFallState() = default;

void
PlayerFallState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("fall");
}

void
PlayerFallState::on_update(float delta)
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if(player->is_on_floor())
    {
        player->switch_state("idle");
        player->on_land();

        play_audio(_T("player_land"), false);
    }
    else if(player->can_attack())
    {
        player->switch_state("attack");
    }
}
