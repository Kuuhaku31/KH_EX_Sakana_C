
// shoot_barb.cpp

#include "enemy_state_nodes.h"

static void
func_shoot()
{
    Enemy*  enemy  = (Enemy*)CharacterManager::instance()->get_enemy();
    Player* player = (Player*)CharacterManager::instance()->get_player();

    const Vector2& player_pos = player->get_position();

    int barb_num = 0;
    if(enemy->get_hp() > 5)
    {
        barb_num = range_random(1, 3);
    }
    else
    {
        barb_num = range_random(3, 6);
    }

    for(int i = 0; i < barb_num; i++)
    {
        Vector2 target = player_pos;
        target.vy -= 30;

        // 随机偏移
        int dy = range_random(-60, 30);
        target.vy += dy;

        int dx = range_random(-200, 200);
        target.vx += dx;

        enemy->shoot_barb(target);
    }

    // 播放音效
    play_audio(_T("enemy_throw_barbs"), false);
}

static void
func_switch()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    int rand_num = range_random(0, 100);
    if(!enemy->is_on_floor())
    {
        enemy->switch_state("fall");
    }
    else if(enemy->get_hp() > 5)
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

EnemyShootBarbState::EnemyShootBarbState()
{
    timer_shoot.set_wait_time(0.5f);
    timer_shoot.set_one_shot(true);
    timer_shoot.set_on_timeout(func_shoot); // 计时结束时生成飞刺球

    timer_switch.set_wait_time(0.8f);
    timer_switch.set_one_shot(true);
    timer_switch.set_on_timeout(func_switch); // 计时结束时切换状态
}

void
EnemyShootBarbState::on_enter()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    // 目前先用 throw_barb 的动画代替
    enemy->set_animation("throw_barb");
    enemy->set_gravity_enable(false); // 禁用重力

    timer_shoot.restart();
    timer_switch.restart();
}

void
EnemyShootBarbState::on_update(float delta)
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();

    timer_shoot.on_update(delta);
    timer_switch.on_update(delta);

    if(enemy->get_hp() <= 0) enemy->switch_state("dead");
}

void
EnemyShootBarbState::on_exit()
{
    Enemy* enemy = (Enemy*)CharacterManager::instance()->get_enemy();
    enemy->set_gravity_enable(true); // 恢复重力
}
