
// fall.cpp

#include "enemy_state_nodes.h"

void
EnemyFallState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->set_animation("fall");    // 设置坠落动画
    enemy->set_gravity_enable(true); // 开启重力
}

void
EnemyFallState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    if(enemy->get_hp() <= 0) enemy->switch_state("dead"); // 如果敌人死亡
    if(enemy->is_on_floor()) enemy->switch_state("idle"); // 如果敌人着地
}
