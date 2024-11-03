
// run //

#include "character_manager.h"
#include "player_state_nodes.h"

PlayerRunState::PlayerRunState()  = default;
PlayerRunState::~PlayerRunState() = default;

void
PlayerRunState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("run");

    play_audio(_T("player_run"), true); // 循环播放跑动音效
}

void
PlayerRunState::on_update(float delta)
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if(player->get_move_axis() == 0)
    {
        player->switch_state("idle");
    }
    else if(player->can_jump())
    {
        player->switch_state("jump");
    }
    else if(player->can_attack())
    {
        player->switch_state("attack");
    }
    else if(player->can_roll())
    {
        player->switch_state("roll");
    }
}

void
PlayerRunState::on_exit()
{
    stop_audio(_T("player_run")); // 停止播放跑动音效
}
