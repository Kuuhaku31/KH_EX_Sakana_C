
// game.cpp

#include "game.h"
#include "write_message.h"

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

    bool is_running      = true;  // 是否运行
    bool is_have_outcome = false; // 是否有结果

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

        // 检测输赢
        switch(CharacterManager::instance()->get_outcome())
        {
        case CharacterManager::Outcome::WIN:
            is_have_outcome = true;
            WriteMessage::instance()->set_message(TxtType::WIN);
            break;
        case CharacterManager::Outcome::LOSE:
            is_have_outcome = true;
            WriteMessage::instance()->set_message(TxtType::DEAD);
            break;
        case CharacterManager::Outcome::NONE:
            break;
        default:
            break;
        }

        // 渲染
        setbkcolor(RGB(0, 0, 0));
        cleardevice();
        draw_background();                             // 绘制背景
        BulletTimeManager::instance()->post_process(); // 后处理
        CharacterManager::instance()->on_render();     // 渲染角色
        draw_remain_hp();                              // 绘制剩余 HP
        draw_bullet_time_energy();                     // 绘制 bullet time 能量条

        // 绘制输赢提示
        if(is_have_outcome)
        {
            WriteMessage::instance()->on_update(scaled_delta);
            WriteMessage::instance()->post_process();

            if(WriteMessage::instance()->is_done())
            {
                // 游戏结束
                Sleep(2000);
                MessageBox(hwnd, _T("Game Over"), _T("Game Over"), MB_OK | MB_ICONINFORMATION);
                is_running = false;
            }
        }

        // const std::vector<CollisionBox*> collision_boxes = CollisionManager::instance()->get_collision_box_list();
        // Debuger::on_debug_render(collision_boxes); // 调试渲染

        FlushBatchDraw();

        // 更新上一帧时间
        last_tick                  = frame_start;
        nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
        if(sleep_duration > nanoseconds::zero()) std::this_thread::sleep_for(sleep_duration);
    }

    EndBatchDraw();

    return 0;
}
