
// player 构造函数，初始化角色的动画、状态机等

#include "player.h"
#include "player_state_nodes.h"
#include "resources_manager.h"

Player::Player()
{
    // 初始化位置，朝向，逻辑高度
    is_facing_left = false;
    position       = { 250, 250 };
    logic_height   = 120;

    // 初始化攻击碰撞箱
    hit_box->set_size({ 150, 150 });
    hit_box->set_layer_src(CollisionLayer::None);
    hit_box->set_layer_dst(CollisionLayer::Enemy);
    hit_box->set_enable(false);

    // 初始化受击碰撞箱
    hurt_box->set_size({ 40, 80 });
    hurt_box->set_layer_src(CollisionLayer::Player);
    hurt_box->set_layer_dst(CollisionLayer::None);
    hurt_box->set_on_collide([&]() { on_hurt(); });

    // 初始化翻滚CD定时器
    timer_roll_cd.set_wait_time(CD_ROLL);
    timer_roll_cd.set_one_shot(true);
    timer_roll_cd.set_on_timeout([&]() {
        is_roll_cd_comp = true;
    });

    // 初始化攻击CD定时器
    timer_attack_cd.set_wait_time(CD_ATTACK);
    timer_attack_cd.set_one_shot(true);
    timer_attack_cd.set_on_timeout([&]() {
        is_attack_cd_comp = true;
    });

    // 初始化动画
    {
        // attack动画
        AnimationGroup& animation_attack = animation_pool["attack"];

        Animation& animation_attack_left = animation_attack.ani_L;
        animation_attack_left.set_interval(0.05f);
        animation_attack_left.set_loop(false);
        animation_attack_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_attack_left.add_frame(ResourcesManager::instance()->find_image("player_attack_left"), 5);

        Animation& animation_attack_right = animation_attack.ani_R;
        animation_attack_right.set_interval(0.05f);
        animation_attack_right.set_loop(false);
        animation_attack_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_attack_right.add_frame(ResourcesManager::instance()->find_image("player_attack_right"), 5);
    }
    {
        // dead动画
        AnimationGroup& animation_dead = animation_pool["dead"];

        Animation& animation_dead_left = animation_dead.ani_L;
        animation_dead_left.set_interval(0.1f);
        animation_dead_left.set_loop(false);
        animation_dead_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_dead_left.add_frame(ResourcesManager::instance()->find_image("player_dead_left"), 6);

        Animation& animation_dead_right = animation_dead.ani_R;
        animation_dead_right.set_interval(0.1f);
        animation_dead_right.set_loop(false);
        animation_dead_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_dead_right.add_frame(ResourcesManager::instance()->find_image("player_dead_right"), 6);
    }
    {
        // fall动画
        AnimationGroup& animation_fall = animation_pool["fall"];

        Animation& animation_fall_left = animation_fall.ani_L;
        animation_fall_left.set_interval(0.15f);
        animation_fall_left.set_loop(true);
        animation_fall_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_fall_left.add_frame(ResourcesManager::instance()->find_image("player_fall_left"), 5);

        Animation& animation_fall_right = animation_fall.ani_R;
        animation_fall_right.set_interval(0.15f);
        animation_fall_right.set_loop(true);
        animation_fall_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_fall_right.add_frame(ResourcesManager::instance()->find_image("player_fall_right"), 5);
    }
    {
        // dile 动画
        AnimationGroup& animation_idle = animation_pool["idle"];

        Animation& animation_idle_left = animation_idle.ani_L;
        animation_idle_left.set_interval(0.15f);
        animation_idle_left.set_loop(true);
        animation_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_idle_left.add_frame(ResourcesManager::instance()->find_image("player_idle_left"), 5);

        Animation& animation_idle_right = animation_idle.ani_R;
        animation_idle_right.set_interval(0.15f);
        animation_idle_right.set_loop(true);
        animation_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_idle_right.add_frame(ResourcesManager::instance()->find_image("player_idle_right"), 5);
    }
    {
        // jump动画
        AnimationGroup& animation_jump = animation_pool["jump"];

        Animation& animation_jump_left = animation_jump.ani_L;
        animation_jump_left.set_interval(0.15f);
        animation_jump_left.set_loop(false);
        animation_jump_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_jump_left.add_frame(ResourcesManager::instance()->find_image("player_jump_left"), 5);

        Animation& animation_jump_right = animation_jump.ani_R;
        animation_jump_right.set_interval(0.15f);
        animation_jump_right.set_loop(false);
        animation_jump_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_jump_right.add_frame(ResourcesManager::instance()->find_image("player_jump_right"), 5);
    }
    {
        // roll 动画
        AnimationGroup& animation_roll = animation_pool["roll"];

        Animation& animation_roll_left = animation_roll.ani_L;
        animation_roll_left.set_interval(0.05f);
        animation_roll_left.set_loop(false);
        animation_roll_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_roll_left.add_frame(ResourcesManager::instance()->find_image("player_roll_left"), 7);

        Animation& animation_roll_right = animation_roll.ani_R;
        animation_roll_right.set_interval(0.05f);
        animation_roll_right.set_loop(false);
        animation_roll_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_roll_right.add_frame(ResourcesManager::instance()->find_image("player_roll_right"), 7);
    }
    {
        // run 动画
        AnimationGroup& animation_run = animation_pool["run"];

        Animation& animation_run_left = animation_run.ani_L;
        animation_run_left.set_interval(0.075f);
        animation_run_left.set_loop(true);
        animation_run_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_run_left.add_frame(ResourcesManager::instance()->find_image("player_run_left"), 10);

        Animation& animation_run_right = animation_run.ani_R;
        animation_run_right.set_interval(0.075f);
        animation_run_right.set_loop(true);
        animation_run_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_run_right.add_frame(ResourcesManager::instance()->find_image("player_run_right"), 10);
    }
    {
        // repulsed 动画，是 roll 的反向动画
        AnimationGroup& animation_repulsed = animation_pool["repulsed"];

        Animation& animation_repulsed_left = animation_repulsed.ani_L;
        animation_repulsed_left.set_interval(0.05f);
        animation_repulsed_left.set_loop(false);
        animation_repulsed_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_repulsed_left.add_frame(ResourcesManager::instance()->find_image("player_roll_left"), 7, true); // 反向

        Animation& animation_repulsed_right = animation_repulsed.ani_R;
        animation_repulsed_right.set_interval(0.05f);
        animation_repulsed_right.set_loop(false);
        animation_repulsed_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_repulsed_right.add_frame(ResourcesManager::instance()->find_image("player_roll_right"), 7, true); // 反向
    }

    // 特效动画
    {
        animation_slash_up.set_interval(0.07f);
        animation_slash_up.set_loop(false);
        animation_slash_up.set_anchor_mode(Animation::AnchorMode::Centered);
        animation_slash_up.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_up"), 5);

        animation_slash_down.set_interval(0.07f);
        animation_slash_down.set_loop(false);
        animation_slash_down.set_anchor_mode(Animation::AnchorMode::Centered);
        animation_slash_down.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_down"), 5);

        animation_slash_left.set_interval(0.07f);
        animation_slash_left.set_loop(false);
        animation_slash_left.set_anchor_mode(Animation::AnchorMode::Centered);
        animation_slash_left.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_left"), 5);

        animation_slash_right.set_interval(0.07f);
        animation_slash_right.set_loop(false);
        animation_slash_right.set_anchor_mode(Animation::AnchorMode::Centered);
        animation_slash_right.add_frame(ResourcesManager::instance()->find_image("player_vfx_attack_right"), 5);

        animation_jump_vfx.set_interval(0.05f);
        animation_jump_vfx.set_loop(false);
        animation_jump_vfx.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_jump_vfx.add_frame(ResourcesManager::instance()->find_image("player_vfx_jump"), 5);
        animation_jump_vfx.set_on_finished([&]() { is_jump_vfx_visible = false; });

        animation_land_vfx.set_interval(0.1f);
        animation_land_vfx.set_loop(false);
        animation_land_vfx.set_anchor_mode(Animation::AnchorMode::BottomCentered);
        animation_land_vfx.add_frame(ResourcesManager::instance()->find_image("player_vfx_land"), 2);
        animation_land_vfx.set_on_finished([&]() { is_land_vfx_visible = false; });
    }

    // 初始化状态机
    {
        state_machine.register_state("attack", new PlayerAttackState());
        state_machine.register_state("dead", new PlayerDeadState());
        state_machine.register_state("fall", new PlayerFallState());
        state_machine.register_state("idle", new PlayerIdleState());
        state_machine.register_state("jump", new PlayerJumpState());
        state_machine.register_state("roll", new PlayerRollState());
        state_machine.register_state("run", new PlayerRunState());
        state_machine.register_state("repulsed", new PlayerRepulsedState());

        // 设置初始状态
        state_machine.set_entry("idle");
    }
}

// 析构函数
Player::~Player() = default;
