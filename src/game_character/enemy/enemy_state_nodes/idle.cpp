
// idle.cpp

#include "enemy_state_nodes.h"

#define PROBABILITY_JUMP 25
#define PROBABILITY_RUN 50
#define PROBABILITY_SQUAT 75
#define PROBABILITY_THROW_SILK 80
#define PROBABILITY_THROW_SWORD 90
// #define PROBABILITY_SHOOT_BARB 100

#define PROBABILITY_JUMP_ 25
#define PROBABILITY_RUN_ 35
#define PROBABILITY_THROW_SWORD_ 50
#define PROBABILITY_SHOOT_BARB_ 70
#define PROBABILITY_THROW_SILK_ 75
#define PROBABILITY_THROW_BARB_ 85

static void
func()
{
    Enemy*  enemy  = (Enemy*)CharacterManager::instance()->get_enemy();
    Player* player = (Player*)CharacterManager::instance()->get_player();

    int distance = (player->get_position() - enemy->get_position()).module();

    int rand_num = range_random(0, 100); // 生成0-100的随机数，用于决定下一个状态
    if(enemy->get_hp() > 5)
    {
        if(rand_num <= PROBABILITY_JUMP)
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
        else if(rand_num <= PROBABILITY_RUN)
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
        else if(rand_num <= PROBABILITY_SQUAT)
        {
            enemy->switch_state("squat");
        }
        else if(rand_num <= PROBABILITY_THROW_SILK && distance < 200)
        {
            enemy->switch_state("throw_silk");
        }
        else if(rand_num <= PROBABILITY_THROW_SWORD)
        {
            enemy->switch_state("throw_sword");
        }
        else
        {
            enemy->switch_state("shoot_barb");
        }
    }
    else
    {
        if(rand_num <= PROBABILITY_JUMP_)
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
        else if(rand_num <= PROBABILITY_RUN_)
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
        else if(rand_num <= PROBABILITY_THROW_SWORD_)
        {
            enemy->switch_state("throw_sword");
        }
        else if(rand_num <= PROBABILITY_SHOOT_BARB_)
        {
            enemy->switch_state("shoot_barb");
        }
        else if(rand_num <= PROBABILITY_THROW_SILK_)
        {
            enemy->switch_state("throw_silk");
        }
        else if(rand_num <= PROBABILITY_THROW_BARB_)
        {
            enemy->switch_state("throw_barb");
        }
        else
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
