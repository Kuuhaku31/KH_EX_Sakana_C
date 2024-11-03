
// dead //

#include "character_manager.h"
#include "player_state_nodes.h"

PlayerDeadState::PlayerDeadState()
{
    timer.set_wait_time(2.0f);
    timer.set_one_shot(true);
    timer.set_on_timeout([&]() {
        // 弹出失败对话框，退出游戏
        MessageBox(GetHWnd(), _T("YOU DEAD!"), _T("Game Over"), MB_OK | MB_ICONINFORMATION);
        exit(0);
    });
}

void
PlayerDeadState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("dead");
    play_audio(_T("player_dead"), false);
}

void
PlayerDeadState::on_update(float delta)
{
    timer.on_update(delta);
}
