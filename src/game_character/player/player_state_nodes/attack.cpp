
// attack //

#include "player_state_nodes.h"

// 攻击状态结束回调逻辑
static void
func()
{
    ((Player*)CharacterManager::instance()->get_player())->set_attacking(false);
}

PlayerAttackState::PlayerAttackState()
{
    timer.set_wait_time(TIME_ATTACK);
    timer.set_one_shot(true);
    timer.set_on_timeout(func);
}

PlayerAttackState::~PlayerAttackState() = default;

void
PlayerAttackState::on_enter()
{
    // 获取玩家实例指针
    Player* p = (Player*)CharacterManager::instance()->get_player();

    // 角色设置
    p->set_animation("attack");         // 改变角色状态
    p->get_hit_box()->set_enable(true); // 启用攻击
    p->set_attacking(true);             // 设置攻击状态
    p->on_attack();                     // 角色攻击时调用
    timer.restart();                    // 重启攻击计时器

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
    timer.on_update(delta);    // 更新计时器
    update_hit_box_position(); // 更新碰撞箱位置

    Player* p = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(p->get_hp() <= 0)
    {
        p->switch_state("dead"); // 当hp<=0时，切换到死亡状态
    }
    else if(!p->get_attacking()) // 攻击状态结束
    {
        if(p->get_velocity().vy > 0)
        {
            p->switch_state("fall"); // 当角色下落时，切换到坠落状态
        }
        else if(p->get_move_axis() == 0)
        {

            p->switch_state("idle"); // 当角色不移动时，切换到站立状态
        }
        else if(p->is_on_floor() && p->get_move_axis() != 0)
        {
            p->switch_state("run"); // 当角色在地面上移动时，切换到奔跑状态
        }
    }
}

void
PlayerAttackState::on_exit()
{
    Player* p = (Player*)CharacterManager::instance()->get_player();

    p->get_hit_box()->set_enable(false); // 关闭攻击碰撞箱
    p->set_attacking(false);             // 设置攻击状态
}

void
PlayerAttackState::update_hit_box_position()
{
    Player* p = (Player*)CharacterManager::instance()->get_player();

    Vector2        pos_center = p->get_logic_center(); // 获取角色逻辑中心
    CollisionBox*  hit_boox   = p->get_hit_box();      // 获取攻击碰撞箱
    const Vector2& size       = hit_boox->get_size();  // 获取碰撞箱大小

    // 根据攻击方向设置碰撞箱位置
    Vector2 pos_hit_box;
    switch(p->get_attack_dir())
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
    hit_boox->set_position(pos_hit_box);
}
