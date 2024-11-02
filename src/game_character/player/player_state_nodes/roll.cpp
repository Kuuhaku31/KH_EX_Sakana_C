
// roll //

#include "character_manager.h"
#include "player_state_nodes.h"

PlayerRollState::PlayerRollState()
{
    timer.set_wait_time(0.75f);
    timer.set_one_shot(true);
    timer.set_on_timeout([&]() {
        ((Player*)CharacterManager::instance()->get_player())->set_rolling(false);
    });
}

PlayerRollState::~PlayerRollState() = default;

void
PlayerRollState::on_enter()
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    player->set_animation("roll");
    player->get_hurt_box()->set_enable(false); // 翻滚时，不受伤害
    player->set_rolling(true);
    player->on_roll();
    timer.restart();

    play_audio(_T("player_roll"), false);
}

void
PlayerRollState::on_update(float delta)
{
    timer.on_update(delta);

    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(!player->get_rolling())
    {
        if(player->get_move_axis() != 0 && player->is_on_floor())
        {
            player->switch_state("run");
        }
        else if(player->can_jump())
        {
            player->switch_state("jump");
        }
        else
        {
            player->switch_state("idle");
        }
    }
}

void
PlayerRollState::on_exit()
{
    CharacterManager::instance()->get_player()->get_hurt_box()->set_enable(true);
}
