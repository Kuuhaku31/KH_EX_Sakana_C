
// enemy.cpp

#include "collision_manager.h"
#include "enemy.h"
#include "enemy_state_nodes.h"
#include "resources_manager.h"

Enemy::Enemy()
{
    // 初始化位置
    is_facing_left = true;
    position       = Vector2{ 1050, 200 };
    logic_height   = 150;

    // 初始攻击碰撞盒
    hit_box->set_size(Vector2{ 50, 80 });
    hit_box->set_layer_src(CollisionLayer::None);
    hit_box->set_layer_dst(CollisionLayer::Player);

    // 初始受伤碰撞盒
    hurt_box->set_size(Vector2{ 100, 180 });
    hurt_box->set_layer_src(CollisionLayer::Enemy);
    hurt_box->set_layer_dst(CollisionLayer::None);
    hurt_box->set_on_collide([&]() {
        if(is_dashing_on_floor)
        {
            is_repulsed = true;
        }
        else
        {
            decrease_hp();
        }
    });

    // 初始化silk碰撞盒
    collision_box_silk = CollisionManager::instance()->create_collision_box();
    collision_box_silk->set_size(Vector2{ 225, 225 });
    collision_box_silk->set_layer_src(CollisionLayer::None);
    collision_box_silk->set_layer_dst(CollisionLayer::Player);
    collision_box_silk->set_enable(false);

    // 初始化动画
    {
        {
            // aim //
            AnimationGroup& animation_aim = animation_pool["aim"];

            Animation& animation_aim_left = animation_aim.ani_L;
            animation_aim_left.set_interval(0.05f);
            animation_aim_left.set_loop(false);
            animation_aim_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_aim_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_aim_left"));

            Animation& animation_aim_right = animation_aim.ani_R;
            animation_aim_right.set_interval(0.05f);
            animation_aim_right.set_loop(false);
            animation_aim_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_aim_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_aim_right"));
        }
        {
            // dash_in_air //
            AnimationGroup& animation_dash_in_air = animation_pool["dash_in_air"];

            Animation& animation_dash_in_air_left = animation_dash_in_air.ani_L;
            animation_dash_in_air_left.set_interval(0.05f);
            animation_dash_in_air_left.set_loop(true);
            animation_dash_in_air_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_dash_in_air_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dash_in_air_left"));

            Animation& animation_dash_in_air_right = animation_dash_in_air.ani_R;
            animation_dash_in_air_right.set_interval(0.05f);
            animation_dash_in_air_right.set_loop(true);
            animation_dash_in_air_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_dash_in_air_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dash_in_air_right"));
        }
        {
            // dash_on_floor //
            AnimationGroup& animation_dash_on_floor = animation_pool["dash_on_floor"];

            Animation& animation_dash_on_floor_left = animation_dash_on_floor.ani_L;
            animation_dash_on_floor_left.set_interval(0.05f);
            animation_dash_on_floor_left.set_loop(true);
            animation_dash_on_floor_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_dash_on_floor_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_dash_on_floor_left"));

            Animation& animation_dash_on_floor_right = animation_dash_on_floor.ani_R;
            animation_dash_on_floor_right.set_interval(0.05f);
            animation_dash_on_floor_right.set_loop(true);
            animation_dash_on_floor_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_dash_on_floor_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_dash_on_floor_right"));
        }
        {
            // fall //
            AnimationGroup& animation_fall = animation_pool["fall"];

            Animation& animation_fall_left = animation_fall.ani_L;
            animation_fall_left.set_interval(0.1f);
            animation_fall_left.set_loop(true);
            animation_fall_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_fall_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_fall_left"));

            Animation& animation_fall_right = animation_fall.ani_R;
            animation_fall_right.set_interval(0.1f);
            animation_fall_right.set_loop(true);
            animation_fall_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_fall_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_fall_right"));
        }
        {
            // idle //
            AnimationGroup& animation_idle = animation_pool["idle"];

            Animation& animation_idle_left = animation_idle.ani_L;
            animation_idle_left.set_interval(0.1f);
            animation_idle_left.set_loop(true);
            animation_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_idle_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_idle_left"));

            Animation& animation_idle_right = animation_idle.ani_R;
            animation_idle_right.set_interval(0.1f);
            animation_idle_right.set_loop(true);
            animation_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_idle_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_idle_right"));
        }
        {
            // jump //
            AnimationGroup& animation_jump = animation_pool["jump"];

            Animation& animation_jump_left = animation_jump.ani_L;
            animation_jump_left.set_interval(0.1f);
            animation_jump_left.set_loop(false);
            animation_jump_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_jump_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_jump_left"));

            Animation& animation_jump_right = animation_jump.ani_R;
            animation_jump_right.set_interval(0.1f);
            animation_jump_right.set_loop(false);
            animation_jump_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_jump_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_jump_right"));
        }
        {
            // run //
            AnimationGroup& animation_run = animation_pool["run"];

            Animation& animation_run_left = animation_run.ani_L;
            animation_run_left.set_interval(0.05f);
            animation_run_left.set_loop(true);
            animation_run_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_run_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_run_left"));

            Animation& animation_run_right = animation_run.ani_R;
            animation_run_right.set_interval(0.05f);
            animation_run_right.set_loop(true);
            animation_run_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_run_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_run_right"));
        }
        {
            // squat //
            AnimationGroup& animation_squat = animation_pool["squat"];

            Animation& animation_squat_left = animation_squat.ani_L;
            animation_squat_left.set_interval(0.05f);
            animation_squat_left.set_loop(false);
            animation_squat_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_squat_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_squat_left"));

            Animation& animation_squat_right = animation_squat.ani_R;
            animation_squat_right.set_interval(0.05f);
            animation_squat_right.set_loop(false);
            animation_squat_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_squat_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_squat_right"));
        }
        {
            // throw_barb //
            AnimationGroup& animation_throw_barb = animation_pool["throw_barb"];

            Animation& animation_throw_barb_left = animation_throw_barb.ani_L;
            animation_throw_barb_left.set_interval(0.1f);
            animation_throw_barb_left.set_loop(false);
            animation_throw_barb_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_throw_barb_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_barb_left"));

            Animation& animation_throw_barb_right = animation_throw_barb.ani_R;
            animation_throw_barb_right.set_interval(0.1f);
            animation_throw_barb_right.set_loop(false);
            animation_throw_barb_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_throw_barb_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_barb_right"));
        }
        {
            // throw_silk //
            AnimationGroup& animation_throw_silk = animation_pool["throw_silk"];

            Animation& animation_throw_silk_left = animation_throw_silk.ani_L;
            animation_throw_silk_left.set_interval(0.1f);
            animation_throw_silk_left.set_loop(true);
            animation_throw_silk_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_throw_silk_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_silk_left"));

            Animation& animation_throw_silk_right = animation_throw_silk.ani_R;
            animation_throw_silk_right.set_interval(0.1f);
            animation_throw_silk_right.set_loop(true);
            animation_throw_silk_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_throw_silk_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_silk_right"));
        }
        {
            // throw_sword //
            AnimationGroup& animation_throw_sword = animation_pool["throw_sword"];

            Animation& animation_throw_sword_left = animation_throw_sword.ani_L;
            animation_throw_sword_left.set_interval(0.05f);
            animation_throw_sword_left.set_loop(false);
            animation_throw_sword_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_throw_sword_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_sword_left"));

            Animation& animation_throw_sword_right = animation_throw_sword.ani_R;
            animation_throw_sword_right.set_interval(0.05f);
            animation_throw_sword_right.set_loop(false);
            animation_throw_sword_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_throw_sword_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_throw_sword_right"));
        }
        {
            // repulsed //
            AnimationGroup& animation_repulsed = animation_pool["repulsed"];

            Animation& animation_repulsed_left = animation_repulsed.ani_L;
            animation_repulsed_left.set_interval(0.05f);
            animation_repulsed_left.set_loop(false);
            animation_repulsed_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_repulsed_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_repulsed_left"));

            Animation& animation_repulsed_right = animation_repulsed.ani_R;
            animation_repulsed_right.set_interval(0.05f);
            animation_repulsed_right.set_loop(false);
            animation_repulsed_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_repulsed_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_repulsed_right"));
        }
        {
            // 特效 //
            animation_silk.set_interval(0.1f);
            animation_silk.set_loop(false);
            animation_silk.set_anchor_mode(Animation::AnchorMode::Centered);
            animation_silk.add_frame(ResourcesManager::instance()->find_atlas("silk"));

            Animation& animation_dash_in_air_left = animation_dash_in_air_vfx.ani_L;
            animation_dash_in_air_left.set_interval(0.1f);
            animation_dash_in_air_left.set_loop(false);
            animation_dash_in_air_left.set_anchor_mode(Animation::AnchorMode::Centered);
            animation_dash_in_air_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_vfx_dash_in_air_left"));

            Animation& animation_dash_in_air_right = animation_dash_in_air_vfx.ani_R;
            animation_dash_in_air_right.set_interval(0.1f);
            animation_dash_in_air_right.set_loop(false);
            animation_dash_in_air_right.set_anchor_mode(Animation::AnchorMode::Centered);
            animation_dash_in_air_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_vfx_dash_in_air_right"));

            Animation& animation_dash_on_floor_left = animation_dash_on_floor_vfx.ani_L;
            animation_dash_on_floor_left.set_interval(0.1f);
            animation_dash_on_floor_left.set_loop(false);
            animation_dash_on_floor_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_dash_on_floor_left.add_frame(ResourcesManager::instance()->find_atlas("enemy_vfx_dash_on_floor_left"));

            Animation& animation_dash_on_floor_right = animation_dash_on_floor_vfx.ani_R;
            animation_dash_on_floor_right.set_interval(0.1f);
            animation_dash_on_floor_right.set_loop(false);
            animation_dash_on_floor_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
            animation_dash_on_floor_right.add_frame(ResourcesManager::instance()->find_atlas("enemy_vfx_dash_on_floor_right"));
        }
    }

    // 初始化状态机
    {
        state_machine.register_state("aim", new EnemyAimState());
        state_machine.register_state("dash_in_air", new EnemyDashInAirState());
        state_machine.register_state("dash_on_floor", new EnemyDashOnFloorState());
        state_machine.register_state("dead", new EnemyDeadState());
        state_machine.register_state("fall", new EnemyFallState());
        state_machine.register_state("idle", new EnemyIdleState());
        state_machine.register_state("jump", new EnemyJumpState());
        state_machine.register_state("run", new EnemyRunState());
        state_machine.register_state("squat", new EnemySquatState());
        state_machine.register_state("throw_barb", new EnemyThrowBarbState());
        state_machine.register_state("throw_silk", new EnemyThrowSilkState());
        state_machine.register_state("throw_sword", new EnemyThrowSwordState());
        state_machine.register_state("repulsed", new EnemyRepulsedState());

        state_machine.set_entry("idle");
    }
}

Enemy::~Enemy()
{
    CollisionManager::instance()->destroy_collision_box(collision_box_silk);
}
