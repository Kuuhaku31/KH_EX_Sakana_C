
// animation.h

#pragma once

#include "atlas.h"
#include "timer.h"
#include "util.h"
#include "vector2.h"

class Animation
{
public:
    enum class AnchorMode
    {
        TopLeft,
        Centered,
        BottomCentered,
    };

public:
    Animation()
    {
        // 默认设置为循环播放
        timer.set_one_shot(false);

        // 设置回调函数
        timer.set_on_timeout(
            [&]() {
                idx_frame++;
                if(idx_frame >= frame_list.size())
                {
                    idx_frame = is_loop ? 0 : frame_list.size() - 1;
                    if(!is_loop && on_finished)
                    {
                        on_finished();
                    }
                }
            });
    }

    ~Animation() = default;

    // 重置
    void
    reset()
    {
        timer.restart();
        idx_frame = 0;
    }

    // 设置渲染中心
    void
    set_anchor_mode(AnchorMode mode)
    {
        anchor_mode = mode;
    }

    // 设置位置
    void
    set_position(const Vector2& pos)
    {
        position = pos;
    }

    // 设置是否循环播放
    void
    set_loop(bool flag)
    {
        is_loop = flag;
    }

    // 设置帧间隔
    void
    set_interval(float interval)
    {
        timer.set_wait_time(interval);
    }

    // 设置动画结束回调
    void
    set_on_finished(std::function<void()> f)
    {
        on_finished = f;
    }

    // 添加帧，利用一个图片切割成多个帧
    void
    add_frame(IMAGE* img, int num, bool is_backward = false)
    {
        int wide = img->getwidth();
        int high = img->getheight();

        int wide_frame = wide / num;
        if(is_backward)
        {
            for(int i = num - 1; i >= 0; i--)
            {
                Rect rect_src = { i * wide_frame, 0, wide_frame, high };
                frame_list.emplace_back(img, rect_src); // 直接在frame_list中创建一个Frame对象
            }
        }
        else
        {
            for(int i = 0; i < num; i++)
            {
                Rect rect_src = { i * wide_frame, 0, wide_frame, high };
                frame_list.emplace_back(img, rect_src); // 直接在frame_list中创建一个Frame对象
            }
        }
    }

    // 添加帧，利用图集添加帧，is_backward 为 true 时，逆序添加
    void
    add_frame(Atlas* atlas, bool is_backward = false)
    {
        if(is_backward)
        {
            for(int i = atlas->get_size() - 1; i >= 0; i--)
            {
                IMAGE* img   = atlas->get_image(i);
                int    img_w = img->getwidth();
                int    img_h = img->getheight();

                Rect rect_src = { 0, 0, img_w, img_h };

                frame_list.emplace_back(img, rect_src);
            }
        }
        else
        {
            for(int i = 0; i < atlas->get_size(); i++)
            {
                IMAGE* img   = atlas->get_image(i);
                int    img_w = img->getwidth();
                int    img_h = img->getheight();

                Rect rect_src = { 0, 0, img_w, img_h };

                frame_list.emplace_back(img, rect_src);
            }
        }
    }

    // 更新
    void
    on_update(float dt)
    {
        timer.on_update(dt);
    }

    // 渲染
    void
    on_render()
    {
        const Frame& frame = frame_list[idx_frame];

        Rect rect_dst;
        rect_dst.w = frame.rect_src.w;
        rect_dst.h = frame.rect_src.h;
        if(anchor_mode == AnchorMode::TopLeft)
        {
            rect_dst.x = (int)position.vx;
            rect_dst.y = (int)position.vy;
        }
        else if(anchor_mode == AnchorMode::Centered)
        {
            rect_dst.x = (int)(position.vx - frame.rect_src.w / 2);
            rect_dst.y = (int)(position.vy - frame.rect_src.h / 2);
        }
        else if(anchor_mode == AnchorMode::BottomCentered)
        {
            rect_dst.x = (int)(position.vx - frame.rect_src.w / 2);
            rect_dst.y = (int)(position.vy - frame.rect_src.h);
        }

        putimage_a(frame.img, rect_dst, frame.rect_src);
    }

private:
    // 帧结构体
    struct Frame
    {
        Rect   rect_src;
        IMAGE* img = nullptr;

        Frame() = default;
        Frame(IMAGE* img, const Rect& rect_src)
            : img(img)
            , rect_src(rect_src)
        {
        }

        ~Frame() = default;
    };

private:
    Timer   timer;
    Vector2 position;
    bool    is_loop   = true;
    size_t  idx_frame = 0;

    std::vector<Frame>    frame_list;
    std::function<void()> on_finished;

    AnchorMode anchor_mode = AnchorMode::Centered;
};
