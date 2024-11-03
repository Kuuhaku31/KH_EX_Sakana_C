
// throw_barb.cpp

#include "enemy_state_nodes.h"

static void
func()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->throw_barbs();
    enemy->switch_state("idle");
}

EnemyThrowBarbState::EnemyThrowBarbState()
{
    timer.set_one_shot(true);
    timer.set_wait_time(0.8f);
    timer.set_on_timeout(func);
}

void
EnemyThrowBarbState::on_enter()
{
    CharacterManager::instance()->get_enemy()->set_animation("throw_barb");

    timer.restart();

    play_audio(_T("enemy_throw_barbs"), false);
}

void
EnemyThrowBarbState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    timer.on_update(delta);

    if(enemy->get_hp() <= 0) enemy->switch_state("dead");
}
