
// timer.h

#pragma once

#include <functional>

class Timer
{
public:
    Timer()  = default;
    ~Timer() = default;

    // 设置等待时间
    void
    restart()
    {
        pass_time = 0;
        shotted   = false;
    }

    // 设置等待时间
    void
    set_wait_time(float t)
    {
        wait_time = t;
    }

    // 设置是否为一次性触发
    void
    set_one_shot(bool flag)
    {
        one_shot = flag;
    }

    // 设置超时回调函数
    void
    set_on_timeout(std::function<void()> f)
    {
        on_timeout = f;
    }

    // 暂停计时器
    void
    pause()
    {
        paused = true;
    }

    // 恢复计时器
    void
    resume()
    {
        paused = false;
    }

    // 更新计时器
    void
    on_update(float dt)
    {
        // 如果计时器暂停，则不更新
        if(paused)
        {
            return;
        }

        // 更新计时器
        pass_time += dt;

        // 如果计时器超时
        if(pass_time >= wait_time)
        {
            // 如果计时器为一次性触发，则不再触发
            bool can_shot = (!one_shot || (one_shot && !shotted));
            shotted       = true;

            // 如果可以触发超时回调函数
            if(can_shot && on_timeout)
            {
                on_timeout();
            }

            // 重置计时器
            pass_time -= wait_time;
        }
    }

private:
    float pass_time = 0;     // 计时器已经过去的时间
    float wait_time = 0;     // 计时器等待的时间
    bool  paused    = false; // 计时器是否暂停
    bool  shotted   = false; // 计时器是否已经触发过一次
    bool  one_shot  = false; // 计时器是否为一次性触发

    // 超时回调函数
    std::function<void()> on_timeout = nullptr;
};
