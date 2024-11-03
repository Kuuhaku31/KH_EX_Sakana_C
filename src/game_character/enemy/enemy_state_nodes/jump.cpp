
// jump.cpp

#include "enemy_state_nodes.h"

void
EnemyJumpState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    enemy->set_animation("jump");                   // 设置跳跃动画
    enemy->set_velocity(Vector2{ 0, -SPEED_JUMP }); // 设置速度
}

void
EnemyJumpState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    if(enemy->get_hp() <= 0)
    {
        enemy->switch_state("dead"); // 如果敌人死亡
    }
    else if(enemy->get_velocity().vy > 0)
    {
        int rand_num = range_random(0, 100); // 生成0-100的随机数，用于决定下一个状态
        if(enemy->get_hp() > 5)
        {
            if(rand_num <= 50)
            {
                enemy->switch_state("aim");
            }
            else if(rand_num <= 80)
            {
                enemy->switch_state("fall");
            }
            else
            {
                enemy->switch_state("throw_silk");
            }
        }
        else
        {
            if(rand_num <= 50)
            {
                enemy->switch_state("throw_silk");
            }
            else if(rand_num <= 80)
            {
                enemy->switch_state("fall");
            }
            else
            {
                enemy->switch_state("aim");
            }
        }
    }
}
