
// attack //

#include "character_manager.h"
#include "player_state_nodes.h"

PlayerAttackState::PlayerAttackState()
{
    timer.set_wait_time(0.3f);
    timer.set_one_shot(true);
    timer.set_on_timeout([&]() {
        Player* player = (Player*)CharacterManager::instance()->get_player();
        player->set_attacking(false);
    });
}

PlayerAttackState::~PlayerAttackState() = default;

void
PlayerAttackState::on_enter()
{
    // 改变角色状态
    CharacterManager::instance()->get_player()->set_animation("attack");

    // 获取玩家实例指针
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 启用攻击
    player->get_hit_box()->set_enable(true);
    player->set_attacking(true);
    player->on_attack();
    timer.restart();

    // 更新碰撞箱位置
    update_hit_box_position();

    // 随机播放攻击音效
    switch(range_random(1, 3))
    {
    case 1:
        play_audio(_T("player_attack_1"), false);
        break;
    case 2:
        play_audio(_T("player_attack_2"), false);
        break;
    case 3:
        play_audio(_T("player_attack_3"), false);
        break;
    }
}

void
PlayerAttackState::on_update(float delta)
{
    timer.on_update(delta);
    update_hit_box_position();

    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    // 当hp<=0时，切换到死亡状态
    if(player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    // 攻击状态结束
    else if(!player->get_attacking())
    {
        if(player->get_velocity().vy > 0)
        {
            // 切换到坠落状态
            player->switch_state("fall");
        }
        else if(player->get_move_axis() == 0)
        {
            // 切换到待机状态
            player->switch_state("idle");
        }
        else if(player->is_on_floor() && player->get_move_axis() != 0)
        {
            // 切换到跑动状态
            player->switch_state("run");
        }
    }
}

void
PlayerAttackState::on_exit()
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 禁用攻击
    player->get_hit_box()->set_enable(false);
    player->set_attacking(false);
}

void
PlayerAttackState::update_hit_box_position()
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    Vector2        pos_center = player->get_logic_center(); // 获取角色逻辑中心
    CollisionBox*  hit_boox   = player->get_hit_box();      // 获取攻击碰撞箱
    const Vector2& size       = hit_boox->get_size();       // 获取碰撞箱大小
    Vector2        pos_hit_box;                             // 碰撞箱位置

    // 根据攻击方向设置碰撞箱位置
    switch(player->get_attack_dir())
    {
    case Player::AttackDir::Up:
        pos_hit_box = { pos_center.vx, pos_center.vy - size.vy / 2 };
        break;

    case Player::AttackDir::Down:
        pos_hit_box = { pos_center.vx, pos_center.vy + size.vy / 2 };
        break;

    case Player::AttackDir::Left:
        pos_hit_box = { pos_center.vx - size.vx / 2, pos_center.vy };
        break;

    case Player::AttackDir::Right:
        pos_hit_box = { pos_center.vx + size.vx / 2, pos_center.vy };
        break;
    }

    // 更新碰撞箱位置
    hit_boox->set_position(pos_hit_box);
}
