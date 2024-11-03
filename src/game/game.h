
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

// 绘制剩余 HP
static void
draw_remain_hp()
{
    static IMAGE* img_ui_heart = ResourcesManager::instance()->find_image("ui_heart");

    Rect rect_dst = { 0, 10, img_ui_heart->getwidth(), img_ui_heart->getheight() };
    for(int i = 0; i < CharacterManager::instance()->get_player()->get_hp(); i++)
    {
        rect_dst.x = 10 + i * 40;
        putimage_ex(img_ui_heart, rect_dst);
    }
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
