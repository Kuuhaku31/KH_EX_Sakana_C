
// player_state_nodes.h
// 玩家状态节点

#pragma once

#include "character_manager.h"

#define TIME_ATTACK 0.3f
#define TIME_ROLL 0.35f

// 玩家攻击状态
class PlayerAttackState : public StateNode
{
public:
    PlayerAttackState();
    ~PlayerAttackState();

    void on_enter() override;
    void on_update(float delta) override;
    void on_exit() override;

private:
    Timer timer;              // 攻击状态持续时间的计时器
    Timer timer_stop_hit_box; // 停止攻击碰撞盒的计时器

    void update_hit_box_position();
};

// 玩家死亡状态
class PlayerDeadState : public StateNode
{
public:
    PlayerDeadState();
    ~PlayerDeadState() = default;

    void on_enter() override;
    void on_update(float delta) override;

private:
    Timer timer;
};

// 玩家坠落状态
class PlayerFallState : public StateNode
{
public:
    PlayerFallState();
    ~PlayerFallState();

    void on_enter() override;
    void on_update(float delta) override;
};

// 玩家受击状态
class PlayerHurtState : public StateNode
{
public:
    PlayerHurtState();
    ~PlayerHurtState() = default;

    void on_enter() override;
    void on_update(float delta) override;
    void on_exit() override;
};

// 玩家跳跃状态
class PlayerJumpState : public StateNode
{
public:
    PlayerJumpState();
    ~PlayerJumpState();

    void on_enter() override;
    void on_update(float delta) override;
};

// 玩家移动状态
class PlayerRunState : public StateNode
{
public:
    PlayerRunState();
    ~PlayerRunState();

    void on_enter() override;
    void on_update(float delta) override;
    void on_exit() override;
};

// 玩家翻滚状态
class PlayerRollState : public StateNode
{
public:
    PlayerRollState();
    ~PlayerRollState();

    void on_enter() override;
    void on_update(float delta) override;
    void on_exit() override;

private:
    Timer timer;
};

// 玩家闲置状态
class PlayerIdleState : public StateNode
{
public:
    PlayerIdleState();
    ~PlayerIdleState();

    void on_enter() override;
    void on_update(float delta) override;
};

// 玩家受击后退状态
class PlayerRepulsedState : public StateNode
{
public:
    PlayerRepulsedState()  = default;
    ~PlayerRepulsedState() = default;

    void on_enter() override;
    void on_update(float delta) override;
};
