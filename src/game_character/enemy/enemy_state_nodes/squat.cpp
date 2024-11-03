
// squat.cpp

#include "enemy_state_nodes.h"

static void
func()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->switch_state("dash_on_floor");
}

EnemySquatState::EnemySquatState()
{
    timer.set_one_shot(true);
    timer.set_wait_time(0.5f);
    timer.set_on_timeout(func);
}

void
EnemySquatState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    enemy->set_animation("squat");
    enemy->set_facing_left(enemy->get_position().vx > CharacterManager::instance()->get_player()->get_position().vx);
    timer.restart();
}

void
EnemySquatState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    timer.on_update(delta);

    if(enemy->get_hp() <= 0) enemy->switch_state("dead");
}
