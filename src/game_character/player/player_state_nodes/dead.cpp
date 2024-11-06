
// dead //

#include "bullet_time_manager.h"
#include "character_manager.h"
#include "player_state_nodes.h"
#include "write_message.h"

PlayerDeadState::PlayerDeadState()
{
    timer.set_wait_time(2.0f);
    timer.set_one_shot(true);
    timer.set_on_timeout([&]() {
        CharacterManager::instance()->set_outcome(CharacterManager::Outcome::LOSE);
    });
}

void
PlayerDeadState::on_enter()
{
    // 退出子弹时间
    BulletTimeManager::instance()->set_status(BulletTimeManager::Status::Exiting);
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
    float vx = player->get_velocity().vx;
    float vt = vx > 0 ? vx - AIR_RESISTANCE : vx + AIR_RESISTANCE;

    if(vx * vt <= 0)
    {
        // 如果速度的方向发生了变化，直接设置速度为0
        player->set_velocity_x(0);
    }
    else
    {
        player->set_velocity_x(vt);
    }
}
