
// write_message.h

#include "resources_manager.h"
#include "write_message.h"

#include <graphics.h>

// 线性插值
inline float
lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

inline void
putimage_a(int dst_x, int dst_y, IMAGE* img, int alpha)
{
    // static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    // 设置透明度
    BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, (BYTE)alpha, AC_SRC_ALPHA };

    AlphaBlend(
        GetImageHDC(GetWorkingImage()),
        dst_x,
        dst_y,
        img->getwidth(),
        img->getheight(),

        GetImageHDC(img),
        0,
        0,
        img->getwidth(),
        img->getheight(),

        blend_func);
}

// 将屏幕变暗 0~1 之间的值，表示进度
inline void
make_dark(float progress)
{
    DWORD* buffer      = GetImageBuffer(nullptr);
    int    buffer_size = getwidth() * getheight();
    for(int i = 0; i < buffer_size; i++)
    {
        DWORD color = buffer[i];

        BYTE r = GetRValue(color) * lerp(1.0f, 0.0f, progress);
        BYTE g = GetGValue(color) * lerp(1.0f, 0.0f, progress);
        BYTE b = GetBValue(color) * lerp(1.0f, 0.0f, progress);

        buffer[i] = RGB(r, g, b) | 0xFF000000;
    }
}

void
write_message(TxtType type, float progress)
{
    static IMAGE* txt = nullptr;

    switch(type)
    {
    case TxtType::DEAD:
        txt = ResourcesManager::instance()->find_image("ui_txt_dead");
        break;
    case TxtType::WIN:
        txt = ResourcesManager::instance()->find_image("ui_txt_win");
        break;
    }

    int win_w = getwidth();
    int win_h = getheight();

    // 居中显示
    int x = (win_w - txt->getwidth()) / 2;
    int y = (win_h - txt->getheight()) / 2;

    int alpha = lerp(0, 255, progress);

    make_dark(progress);
    putimage_a(x, y, txt, alpha);
}

WriteMessage* WriteMessage::ins = nullptr;
