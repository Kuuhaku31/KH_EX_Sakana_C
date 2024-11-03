
// util.h

#pragma once

#include <graphics.h>

// 连接库
#pragma comment(lib, "WINMM.lib")   // 多媒体库
#pragma comment(lib, "MSIMG32.lib") // 图像库

struct Rect
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

inline void
putimage_ex(IMAGE* img, const Rect& rect_dst, const Rect& rect_src = Rect{ 0, 0, 0, 0 })
{
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    AlphaBlend(
        GetImageHDC(GetWorkingImage()),
        rect_dst.x,
        rect_dst.y,
        rect_dst.w,
        rect_dst.h,

        GetImageHDC(img),
        rect_src.x,
        rect_src.y,
        rect_src.w ? rect_src.w : img->getwidth(),
        rect_src.h ? rect_src.h : img->getheight(),

        blend_func);
}

inline void
load_audio(LPCTSTR path, LPCTSTR id)
{

    static TCHAR str_cmd[MAX_PATH];
    _stprintf_s(str_cmd, _T("open %s alias %s"), path, id);
    mciSendString(str_cmd, NULL, 0, NULL);
}

inline void
play_audio(LPCTSTR id, bool is_loop = false)
{
    static TCHAR str_cmd[MAX_PATH];
    _stprintf_s(str_cmd, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
    mciSendString(str_cmd, NULL, 0, NULL);
}

inline void
stop_audio(LPCTSTR id)
{
    static TCHAR str_cmd[MAX_PATH];
    _stprintf_s(str_cmd, _T("stop %s"), id);
    mciSendString(str_cmd, NULL, 0, NULL);
}

inline int
range_random(int min, int max)
{
    return min + rand() % (max - min + 1);
}
