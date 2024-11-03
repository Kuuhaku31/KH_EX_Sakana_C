
// run.cpp

#include "enemy_state_nodes.h"

void
EnemyRunState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->set_animation("run"); // 设置奔跑动画

    play_audio(_T("enemy_run"), true); // 循环播放奔跑音效
}

void
EnemyRunState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    int dx = enemy->get_position().vx - CharacterManager::instance()->get_player()->get_position().vx;
    enemy->set_velocity({ dx > 0 ? -SPEED_RUN : SPEED_RUN, 0 }); // 根据玩家位置调整敌人速度

    if(enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
    else if(abs(dx) < MIN_DIS)
    {
        int rand_num = range_random(0, 100); // 生成0-100的随机数，用于决定下一个状态
        if(enemy->get_hp() > 5)
        {
            if(rand_num <= 75)
            {
                enemy->switch_state("squat");
            }
            else
            {
                enemy->switch_state("throw_silk");
            }
        }
        else
        {
            if(rand_num <= 75)
            {
                enemy->switch_state("throw_silk");
            }
            else
            {
                enemy->switch_state("squat");
            }
        }
    }
}

void
EnemyRunState::on_exit()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->set_velocity(Vector2{ 0, 0 }); // 速度归零

    stop_audio(_T("enemy_run")); // 停止播放奔跑音效
}
