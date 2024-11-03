
// thorw_sword.cpp

#include "enemy_state_nodes.h"

static void
func_throw()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->throw_swords();

    play_audio(_T("enemy_throw_sword"), false);
}

static void
func_switch()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    int rand_num = range_random(0, 100);
    if(enemy->get_hp() > 5)
    {
        if(rand_num <= 50) // 50%
            enemy->switch_state("squat");
        else if(rand_num <= 80) // 30%
            enemy->switch_state("jump");
        else // 20%
            enemy->switch_state("idle");
    }
    else
    {
        if(rand_num <= 50) // 50%
        {
            enemy->switch_state("jump");
        }
        else if(rand_num <= 80) // 30%
        {
            enemy->switch_state("throw_silk");
        }
        else // 20%
        {
            enemy->switch_state("idle");
        }
    }
}

EnemyThrowSwordState::EnemyThrowSwordState()
{
    timer_throw.set_wait_time(0.65f);
    timer_throw.set_one_shot(true);
    timer_throw.set_on_timeout(func_throw); // 计时结束时生成飞剑

    timer_switch.set_wait_time(1.0f);
    timer_switch.set_one_shot(true);
    timer_switch.set_on_timeout(func_switch); // 计时结束时切换状态
}

void
EnemyThrowSwordState::on_enter()
{
    CharacterManager::instance()->get_enemy()->set_animation("throw_sword");

    timer_throw.restart();
    timer_switch.restart();
}

void
EnemyThrowSwordState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    timer_throw.on_update(delta);
    timer_switch.on_update(delta);

    if(enemy->get_hp() <= 0) enemy->switch_state("dead");
}
