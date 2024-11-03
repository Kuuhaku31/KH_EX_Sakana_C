
// game.h

#pragma once

#include "bullet_time_manager.h"
#include "character_manager.h"
#include "collision_manager.h"
#include "resources_manager.h"
#include "util.h"

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
run()
{
    using namespace std::chrono;

    HWND hwnd = initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetWindowText(hwnd, WINDOW_TITLE);

    // 加载资源
    try
    {
        ResourcesManager::instance()->load();
    }
    catch(const LPCTSTR id)
    {
        TCHAR err_msg[MAX_PATH];
        _stprintf_s(err_msg, _T("Load resource failed: %s"), id);
        MessageBox(hwnd, err_msg, _T("Error"), MB_OK | MB_ICONERROR);
        return -1;
    }

    // 循环播放背景音乐
    play_audio(_T("bgm"), true);

    // 每帧的持续时间（144帧每秒）
    const nanoseconds        frame_duration(1000000000 / GAME_FPS);
    steady_clock::time_point last_tick = steady_clock::now();

    ExMessage msg;

    bool is_running = true;

    BeginBatchDraw();

    while(is_running)
    {
        // 处理消息
        while(peekmessage(&msg)) CharacterManager::instance()->on_input(msg);

        // 计算帧间隔
        steady_clock::time_point frame_start = steady_clock::now();
        duration<float>          delta_time  = frame_start - last_tick;

        // 更新
        float scaled_delta = BulletTimeManager::instance()->on_update(delta_time.count()); // 利用 BulletTimeManager 更新 delta 时间
        CharacterManager::instance()->on_update(scaled_delta);                             // 更新角色
        CollisionManager::instance()->process_collide();                                   // 处理碰撞

        // 渲染
        setbkcolor(RGB(0, 0, 0));
        cleardevice();
        draw_background();                         // 绘制背景
        CharacterManager::instance()->on_render(); // 渲染角色
        draw_remain_hp();                          // 绘制剩余 HP

        // CollisionManager::instance()->on_debug_render(); // 渲染碰撞盒

        FlushBatchDraw();

        // 更新上一帧时间
        last_tick                  = frame_start;
        nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
        if(sleep_duration > nanoseconds::zero()) std::this_thread::sleep_for(sleep_duration);
    }

    EndBatchDraw();

    return 0;
}
