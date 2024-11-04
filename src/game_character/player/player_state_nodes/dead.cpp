
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
    Player* player = (Player*)CharacterManager::instance()->get_player();

    timer.on_update(delta);

    // 空气阻力
    const float AIR_RESISTANCE = 10;

    // 减速
    Vector2 v  = player->get_velocity();
    float   vt = v.vx > 0 ? v.vx - AIR_RESISTANCE : v.vx + AIR_RESISTANCE;

    if(v.vx * vt <= 0)
    {
        // 如果速度的方向发生了变化，直接设置速度为0
        player->set_velocity(Vector2{ 0, 0 });
    }
    else
    {
        v.vx = vt;
        player->set_velocity(v);
    }
}
