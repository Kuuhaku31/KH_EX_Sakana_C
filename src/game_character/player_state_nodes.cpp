
// player_state_nodes.cpp

#include "character_manager.h"
#include "player_state_nodes.h"

// attack //

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


// dead //

PlayerDeadState::PlayerDeadState()
{
    timer.set_wait_time(2.0f);
    timer.set_one_shot(true);
    timer.set_on_timeout([&]() {
        // 弹出失败对话框，退出游戏
        MessageBox(GetHWnd(), _T("Game Over!"), _T("挑战失败！"), MB_OK);
        exit(0);
    });
}

void
PlayerDeadState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("dead");
    play_audio(_T("player_dead"), false);
}

void
PlayerDeadState::on_update(float delta)
{
    timer.on_update(delta);
}


// fall //

PlayerFallState::PlayerFallState()  = default;
PlayerFallState::~PlayerFallState() = default;

void
PlayerFallState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("fall");
}

void
PlayerFallState::on_update(float delta)
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if(player->is_on_floor())
    {
        player->switch_state("idle");
        player->on_land();

        play_audio(_T("player_land"), false);
    }
    else if(player->can_attack())
    {
        player->switch_state("attack");
    }
}


// idle //

PlayerIdleState::PlayerIdleState()  = default;
PlayerIdleState::~PlayerIdleState() = default;

void
PlayerIdleState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("idle");
}

void
PlayerIdleState::on_update(float delta)
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if(player->can_attack())
    {
        player->switch_state("attack");
    }
    else if(player->get_velocity().vx > 0)
    {
        player->switch_state("fall");
    }
    else if(player->can_jump())
    {
        player->switch_state("jump");
    }
    else if(player->can_roll())
    {
        player->switch_state("roll");
    }
    else if(player->get_move_axis() != 0 && player->is_on_floor())
    {
        player->switch_state("run");
    }
}


// jump //

PlayerJumpState::PlayerJumpState()  = default;
PlayerJumpState::~PlayerJumpState() = default;

void
PlayerJumpState::on_enter()
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    player->set_animation("jump");
    player->on_jump();

    play_audio(_T("player_jump"), false);
}

void
PlayerJumpState::on_update(float delta)
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if(player->get_velocity().vy > 0)
    {
        player->switch_state("fall");
    }
    else if(player->can_attack())
    {
        player->switch_state("attack");
    }
}


// roll //

PlayerRollState::PlayerRollState()
{
    timer.set_wait_time(0.75f);
    timer.set_one_shot(true);
    timer.set_on_timeout([&]() {
        ((Player*)CharacterManager::instance()->get_player())->set_rolling(false);
    });
}

PlayerRollState::~PlayerRollState() = default;

void
PlayerRollState::on_enter()
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    player->set_animation("roll");
    player->get_hurt_box()->set_enable(false); // 翻滚时，不受伤害
    player->set_rolling(true);
    player->on_roll();
    timer.restart();

    play_audio(_T("player_roll"), false);
}

void
PlayerRollState::on_update(float delta)
{
    timer.on_update(delta);

    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(!player->get_rolling())
    {
        if(player->get_move_axis() != 0 && player->is_on_floor())
        {
            player->switch_state("run");
        }
        else if(player->can_jump())
        {
            player->switch_state("jump");
        }
        else
        {
            player->switch_state("idle");
        }
    }
}

void
PlayerRollState::on_exit()
{
    CharacterManager::instance()->get_player()->get_hurt_box()->set_enable(true);
}


// run //

PlayerRunState::PlayerRunState()  = default;
PlayerRunState::~PlayerRunState() = default;

void
PlayerRunState::on_enter()
{
    CharacterManager::instance()->get_player()->set_animation("run");

    play_audio(_T("player_run"), true); // 循环播放跑动音效
}

void
PlayerRunState::on_update(float delta)
{
    Player* player = (Player*)CharacterManager::instance()->get_player();

    // 状态跳出逻辑
    if(player->get_hp() <= 0)
    {
        player->switch_state("dead");
    }
    else if(player->get_move_axis() == 0)
    {
        player->switch_state("idle");
    }
    else if(player->can_jump())
    {
        player->switch_state("jump");
    }
    else if(player->can_attack())
    {
        player->switch_state("attack");
    }
    else if(player->can_roll())
    {
        player->switch_state("roll");
    }
}

void
PlayerRunState::on_exit()
{
    play_audio(_T("player_run"), false); // 停止播放跑动音效
}
