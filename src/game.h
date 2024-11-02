
// game.h

#pragma once

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

    // 每帧的持续时间（144帧每秒）
    const nanoseconds        frame_duration(1000000000 / GAME_FPS);
    steady_clock::time_point last_tick = steady_clock::now();

    ExMessage msg;

    bool is_running = true;

    BeginBatchDraw();

    while(is_running)
    {
        while(peekmessage(&msg))
        {
            // 处理消息
            CharacterManager::instance()->on_input(msg);
        }

        steady_clock::time_point frame_start = steady_clock::now();
        duration<float>          delta_time  = frame_start - last_tick;

        // 更新
        CharacterManager::instance()->on_update(delta_time.count());
        CollisionManager::instance()->process_collide();

        setbkcolor(RGB(0, 0, 0));
        cleardevice();

        // 渲染
        draw_background();
        CharacterManager::instance()->on_render();
        CollisionManager::instance()->on_debug_render();

        FlushBatchDraw();

        last_tick                  = frame_start;
        nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
        if(sleep_duration > nanoseconds::zero())
        {
            std::this_thread::sleep_for(sleep_duration);
        }
    }

    EndBatchDraw();

    return 0;
}
