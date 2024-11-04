
// dash_on_floor.cpp

#include "enemy_state_nodes.h"

static void
func()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->set_dash_on_floor(false); // 清除地面冲刺标志
}

EnemyDashOnFloorState::EnemyDashOnFloorState()
{
    timer.set_one_shot(true);
    timer.set_wait_time(0.5f);
    timer.set_on_timeout(func);
}

void
EnemyDashOnFloorState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    enemy->set_animation("dash_on_floor");                                                  // 设置地面冲刺动画
    enemy->set_velocity(Vector2{ enemy->get_facing_left() ? -SPEED_DASH : SPEED_DASH, 0 }); // 设置速度
    enemy->set_dash_on_floor(true);                                                         // 设置地面冲刺标志
    enemy->on_dash();                                                                       // 执行地面冲刺
    timer.restart();                                                                        // 重启定时器

    play_audio(_T("enemy_dash"), false);
}

void
EnemyDashOnFloorState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    timer.on_update(delta); // 更新定时器

    if(enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead"); // 如果敌人死亡
    }
    else if(!enemy->get_dash_on_floor())
    {
        enemy->switch_state("idle"); // 如果敌人不再地面冲刺
    }
    else if(enemy->get_repulsed())
    {
        timer.shot();                    // 立即触发定时器
        enemy->switch_state("repulsed"); // 如果敌人被击退
    }
}
