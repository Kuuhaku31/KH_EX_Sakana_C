
// atlas.h

#pragma once

#include <cstdio>
#include <graphics.h>
#include <vector>

class Atlas
{
public:
    Atlas()  = default;
    ~Atlas() = default;

    // 加载图集
    void
    load(LPCTSTR path_template, int num)
    {
        img_list.clear();
        img_list.resize(num);

        TCHAR path_file[MAX_PATH];
        for(int i = 0; i < num; i++)
        {
            // 生成文件路径，加载图片
            _stprintf_s(path_file, path_template, i + 1);
            loadimage(&img_list[i], path_file);
        }
    }

    // 清空图集
    void
    clear()
    {
        img_list.clear();
    }

    // 获取图片数量
    int
    get_size() const
    {
        return img_list.size();
    }

    // 获取图片
    IMAGE*
    get_image(int idx)
    {
        if(idx < 0 || idx >= img_list.size())
        {
            return nullptr;
        }
        else
        {
            return &img_list[idx];
        }
    }

    // 添加图片
    void
    add_image(const IMAGE& img)
    {
        img_list.push_back(img);
    }

private:
    std::vector<IMAGE> img_list;
};
