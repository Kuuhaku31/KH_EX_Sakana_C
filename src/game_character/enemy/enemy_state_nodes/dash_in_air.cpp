
// dash_in_air.cpp

#include "enemy_state_nodes.h"

void
EnemyDashInAirState::on_enter()
{
    Enemy*        enemy  = (Enemy*)CharacterManager::instance()->get_enemy();
    const Player* player = (Player*)CharacterManager::instance()->get_player();

    Vector2 pos_target = player->get_position(); // 获取玩家位置
    enemy->set_facing_left(pos_target);          // 设置面向玩家

    enemy->set_animation("dash_in_air");                                             // 设置空中冲刺动画
    enemy->set_velocity((pos_target - enemy->get_position()).tounit() * SPEED_DASH); // 设置速度
    enemy->set_gravity_enable(false);                                                // 关闭重力
    enemy->set_dash_in_air(true);                                                    // 设置空中冲刺标志
    enemy->on_dash();                                                                // 执行空中冲刺

    play_audio(_T("enemy_dash"), false);
}

void
EnemyDashInAirState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    if(enemy->get_hp() <= 0) enemy->switch_state("dead"); // 如果敌人死亡
    if(enemy->is_on_floor()) enemy->switch_state("idle"); // 如果敌人着地
}

void
EnemyDashInAirState::on_exit()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    enemy->set_dash_in_air(false);   // 清除空中冲刺标志
    enemy->set_gravity_enable(true); // 重力开启
}
