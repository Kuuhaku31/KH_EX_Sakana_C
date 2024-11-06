
// game.h

#pragma once

#include "bullet_time_manager.h"
#include "character_manager.h"
#include "collision_manager.h"
#include "resources_manager.h"

#include <chrono>
#include <thread>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE _T("Game")

#define GAME_FPS 144

class Debuger
{
public:
    static void on_debug_render(const std::vector<CollisionBox*>& collision_boxes)
    {
        // 依次渲染所有碰撞箱
        for(CollisionBox* box : collision_boxes)
        {
            setlinecolor(box->enable ? 0x00FF00 : 0xFF0000);
            rectangle(
                box->position.vx - box->size.vx / 2,
                box->position.vy - box->size.vy / 2,
                box->position.vx + box->size.vx / 2,
                box->position.vy + box->size.vy / 2);
        }
    }
};

// 绘制剩余 HP
static void
draw_remain_hp()
{
    static IMAGE* img_ui_heart      = ResourcesManager::instance()->find_image("ui_heart");
    static IMAGE* img_ui_hear_enemy = ResourcesManager::instance()->find_image("ui_heart_enemy");

    Rect rect_dst = { 0, 10, img_ui_heart->getwidth(), img_ui_heart->getheight() };
    for(int i = 0; i < CharacterManager::instance()->get_player()->get_hp(); i++)
    {
        rect_dst.x = 10 + i * 40;
        putimage_ex(img_ui_heart, rect_dst);
    }

    rect_dst = { WINDOW_WIDTH - img_ui_hear_enemy->getwidth(), 10, img_ui_hear_enemy->getwidth(), img_ui_hear_enemy->getheight() };
    for(int i = 0; i < CharacterManager::instance()->get_enemy()->get_hp(); i++)
    {
        rect_dst.x = WINDOW_WIDTH - 10 - (i + 1) * 40;
        putimage_ex(img_ui_hear_enemy, rect_dst);
    }
}

// 绘制 bullet time 能量条
static void
draw_bullet_time_energy()
{
    float max_energy = BulletTimeManager::instance()->get_max_energy();
    float energy     = BulletTimeManager::instance()->get_energy();

    int width     = 200;
    int height    = 20;
    int bar_width = width * energy / max_energy;

    setfillcolor(RED);
    solidrectangle(10, WINDOW_HEIGHT - 10 - height, 10 + width, WINDOW_HEIGHT - 10);
    setfillcolor(GREEN);
    solidrectangle(10, WINDOW_HEIGHT - 10 - height, 10 + bar_width, WINDOW_HEIGHT - 10);
}

// 绘制背景
static void
draw_background()
{
    // 通过 ResourcesManager 获取背景图片
    static IMAGE* img_background = ResourcesManager::instance()->find_image("background");
    static Rect   rect_dst       = {
        (getwidth() - img_background->getwidth()) / 2,
        (getheight() - img_background->getheight()) / 2,
        img_background->getwidth(),
        img_background->getheight()
    };

    putimage_ex(img_background, rect_dst);
}

int
run();
