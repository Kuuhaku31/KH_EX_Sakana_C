
// player_state_nodes.h
// 玩家状态节点

#pragma once

#include "state_node.h"
#include "timer.h"

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
    Timer timer;

    void update_hit_box_position();
};

// 玩家死亡状态
class PlayerDeadState : public StateNode
{
public:
    PlayerDeadState();
    ~PlayerDeadState();

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
    ~PlayerHurtState();

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
