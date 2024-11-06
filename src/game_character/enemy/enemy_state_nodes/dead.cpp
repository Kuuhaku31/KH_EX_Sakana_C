
// dead.cpp

#include "enemy_state_nodes.h"
#include "write_message.h"

EnemyDeadState::EnemyDeadState()
{
    timer.set_wait_time(2.0f);
    timer.set_one_shot(true);
    timer.set_on_timeout([&]() {
        CharacterManager::instance()->set_outcome(CharacterManager::Outcome::WIN);
    });
}

void
EnemyDeadState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->set_animation("dead");
    enemy->get_hit_box()->set_enable(false);
    enemy->get_hurt_box()->set_enable(false);

    timer.restart();
}

void
EnemyDeadState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    timer.on_update(delta);

    // 空气阻力
    const float AIR_RESISTANCE = 10;

    // 减速
    float vx = enemy->get_velocity().vx;
    float vt = vx > 0 ? vx - AIR_RESISTANCE : vx + AIR_RESISTANCE;

    if(vx * vt <= 0)
    {
        // 如果速度的方向发生了变化，直接设置速度为0
        enemy->set_velocity_x(0);
    }
    else
    {
        enemy->set_velocity_x(vt);
    }
}
