
// throw_silk.cpp

#include "enemy_state_nodes.h"

static void
func()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    enemy->set_gravity_enable(true); // 重力开启
    enemy->set_throwing_silk(false); // 关闭扔丝线标志
    if(!enemy->is_on_floor() && enemy->get_hp() > 5 && range_random(0, 100) <= 25)
    {
        enemy->switch_state("aim");
    }
    else if(!enemy->is_on_floor())
    {
        enemy->switch_state("fall");
    }
    else
    {
        enemy->switch_state("idle");
    }
}

EnemyThrowSilkState::EnemyThrowSilkState()
{
    timer.set_one_shot(true);
    timer.set_wait_time(0.9f);
    timer.set_on_timeout(func);
}

void
EnemyThrowSilkState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    enemy->set_animation("throw_silk");
    enemy->set_gravity_enable(false);
    enemy->set_throwing_silk(true);
    enemy->set_velocity(Vector2{ 0, 0 });
    enemy->on_throw_silk();

    timer.restart();

    play_audio(_T("enemy_throw_silk"), false);
}

void
EnemyThrowSilkState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    timer.on_update(delta);

    if(enemy->get_hp() <= 0) enemy->switch_state("dead");
}
