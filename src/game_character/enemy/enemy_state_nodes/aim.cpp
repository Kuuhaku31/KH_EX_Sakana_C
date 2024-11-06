
// aim //

#include "enemy_state_nodes.h"

void
func()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->switch_state("dash_in_air"); // 切换到空中冲刺状态
}

EnemyAimState::EnemyAimState()
{
    timer.set_one_shot(true);
    timer.set_wait_time(0.5f);
    timer.set_on_timeout(func);
}

void
EnemyAimState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    enemy->set_animation("aim");          // 设置瞄准动画
    enemy->set_gravity_enable(false);     // 关闭重力
    enemy->set_velocity(Vector2{ 0, 0 }); // 速度归零
    timer.restart();                      // 重启定时器
}

void
EnemyAimState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    timer.on_update(delta); // 更新定时器

    if(enemy->get_hp() <= 0) enemy->switch_state("dead"); // 如果敌人死亡
}

void
EnemyAimState::on_exit()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->set_gravity_enable(true); // 重力开启
}
