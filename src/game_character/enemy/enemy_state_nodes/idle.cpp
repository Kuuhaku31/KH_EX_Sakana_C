
// idle.cpp

#include "enemy_state_nodes.h"

static void
func()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    int rand_num = range_random(0, 100); // 生成0-100的随机数，用于决定下一个状态
    if(enemy->get_hp() > 5)
    {
        if(rand_num <= 25) // 25%
        {
            if(enemy->is_on_floor())
            {
                enemy->switch_state("jump");
            }
            else
            {
                enemy->switch_state("fall");
            }
        }
        else if(rand_num <= 50) // 25%
        {
            if(enemy->is_on_floor())
            {
                enemy->switch_state("run");
            }
            else
            {
                enemy->switch_state("fall");
            }
        }
        else if(rand_num <= 80) // 30%
        {
            enemy->switch_state("squat");
        }
        else if(rand_num <= 90) // 10%
        {
            enemy->switch_state("throw_silk");
        }
        else // 10%
        {
            enemy->switch_state("throw_sword");
        }
    }
    else
    {
        if(rand_num <= 25) // 25%
        {
            if(enemy->is_on_floor())
            {
                enemy->switch_state("jump");
            }
            else
            {
                enemy->switch_state("fall");
            }
        }
        else if(rand_num <= 60) // 35%
        {
            enemy->switch_state("throw_sword");
        }
        else if(rand_num <= 70) // 10%
        {
            enemy->switch_state("throw_silk");
        }
        else if(rand_num <= 90) // 20%
        {
            enemy->switch_state("throw_barb");
        }
        else // 10%
        {
            enemy->switch_state("squat");
        }
    }
}

EnemyIdleState::EnemyIdleState()
{
    timer.set_one_shot(true);
    timer.set_on_timeout(func);
}

void
EnemyIdleState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    enemy->set_animation("idle");         // 设置闲置动画
    enemy->set_velocity(Vector2{ 0, 0 }); // 速度归零

    float wait_time = 0;
    if(enemy->get_hp() > 5)
    {
        wait_time = range_random(0, 2) * 0.25f; // 0-0.5s
    }
    else
    {
        wait_time = range_random(0, 1) * 0.25f; // 0-0.25s
    }

    timer.set_wait_time(wait_time);
    timer.restart();
}

void
EnemyIdleState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    timer.on_update(delta); // 更新定时器

    if(enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead");
    }
    else if(enemy->get_velocity().vy > 0)
    {
        enemy->switch_state("fall");
    }
}

void
EnemyIdleState::on_exit()
{
    Enemy*        enemy  = (Enemy*)CharacterManager::instance()->get_enemy();
    const Player* player = (Player*)CharacterManager::instance()->get_player();

    enemy->set_facing_left(player->get_position().vx < enemy->get_position().vx);
}
