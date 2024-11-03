
// enemy_state_nodes.h
/*

敌人状态节点

* 瞄准 Aim
    ->死亡
    ->空中冲刺

* 空中冲刺 DashInAir
    ->死亡
    ->闲置

* 地面冲刺 DashOnFloor
    ->死亡
    ->闲置

* 坠落 Fall
    ->死亡
    ->闲置

* 闲置 Idle
    ->死亡
    ->坠落
    ->跳跃
    ->奔跑
    ->下蹲
    ->扔刺球
    ->扔丝线
    ->扔剑

* 跳跃 Jump
    ->死亡
    ->瞄准
    ->坠落
    ->扔丝线

* 奔跑 Run
    ->死亡
    ->下蹲
    ->扔丝线

* 下蹲 Squat
    ->死亡
    ->地面冲刺

* 扔刺球 ThrowBarb
    ->死亡
    ->闲置

* 扔丝线 ThrowSilk
    ->死亡
    ->瞄准
    ->坠落
    ->闲置

* 扔剑 ThrowSword
    ->死亡
    ->下蹲
    ->跳跃
    ->闲置
    ->扔丝线

* 死亡 Dead

*/

#pragma once

#include "character_manager.h"
#include "enemy.h"
#include "state_node.h"
#include "timer.h"


// 敌人瞄准状态
class EnemyAimState : public StateNode
{
public:
    EnemyAimState();
    ~EnemyAimState() = default;

    void on_enter() override;
    void on_update(float delta) override;

private:
    Timer timer;
};

// 敌人空中冲刺状态
class EnemyDashInAirState : public StateNode
{
public:
    EnemyDashInAirState()  = default;
    ~EnemyDashInAirState() = default;

    void on_enter() override;
    void on_update(float delta) override;
    void on_exit() override;

private:
    const float SPEED_DASH = 1500.0f;
};

// 敌人地面冲刺状态
class EnemyDashOnFloorState : public StateNode
{
public:
    EnemyDashOnFloorState();
    ~EnemyDashOnFloorState() = default;

    void on_enter() override;
    void on_update(float delta) override;

private:
    const float SPEED_DASH = 1000.0f;

private:
    Timer timer;
};

// 敌人死亡状态
class EnemyDeadState : public StateNode
{
public:
    EnemyDeadState()  = default;
    ~EnemyDeadState() = default;

    void on_enter() override;
};

// 敌人坠落状态
class EnemyFallState : public StateNode
{
public:
    EnemyFallState()  = default;
    ~EnemyFallState() = default;

    void on_enter() override;
    void on_update(float delta) override;
};

// 敌人闲置状态
class EnemyIdleState : public StateNode
{
public:
    EnemyIdleState();
    ~EnemyIdleState() = default;

    void on_enter() override;
    void on_update(float delta) override;
    void on_exit() override;

private:
    Timer timer;
};

// 敌人跳跃状态
class EnemyJumpState : public StateNode
{
public:
    EnemyJumpState()  = default;
    ~EnemyJumpState() = default;

    void on_enter() override;
    void on_update(float delta) override;

private:
    const float SPEED_JUMP = 1000.0f;
};

// 敌人奔跑状态
class EnemyRunState : public StateNode
{
public:
    EnemyRunState()  = default;
    ~EnemyRunState() = default;

    void on_enter() override;
    void on_update(float delta) override;
    void on_exit() override;

private:
    const float MIN_DIS   = 350.0f;
    const float SPEED_RUN = 500.0f;
};

// 敌人下蹲状态
class EnemySquatState : public StateNode
{
public:
    EnemySquatState();
    ~EnemySquatState() = default;

    void on_enter() override;
    void on_update(float delta) override;

private:
    Timer timer;
};

// 敌人扔刺球状态
class EnemyThrowBarbState : public StateNode
{
public:
    EnemyThrowBarbState();
    ~EnemyThrowBarbState() = default;

    void on_enter() override;
    void on_update(float delta) override;

private:
    Timer timer;
};

// 敌人扔丝线状态
class EnemyThrowSilkState : public StateNode
{
public:
    EnemyThrowSilkState();
    ~EnemyThrowSilkState() = default;

    void on_enter() override;
    void on_update(float delta) override;

private:
    Timer timer;
};

// 敌人扔剑状态
class EnemyThrowSwordState : public StateNode
{
public:
    EnemyThrowSwordState();
    ~EnemyThrowSwordState() = default;

    void on_enter() override;
    void on_update(float delta) override;

private:
    Timer timer_throw;
    Timer timer_switch;
};