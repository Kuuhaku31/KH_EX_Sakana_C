
// resource_manager.h

#pragma once

#include "atlas.h"

#include <string>
#include <unordered_map>

class ResourcesManager
{
public:
    static ResourcesManager* instance();

    void load();

    Atlas* find_atlas(const std::string& id) const;
    IMAGE* find_image(const std::string& id) const;


private:
    ResourcesManager();
    ~ResourcesManager();

    void flip_image(IMAGE* src_img, IMAGE* dst_img, int num = 1);
    void flip_image(const std::string& src_id, const std::string& dst_id, int num = 1);
    void flip_atlas(const std::string& src_id, const std::string& dst_id);

private:
    static ResourcesManager* manager; // 单例

    // 利用 unordered_map 来存储资源 //

    std::unordered_map<std::string, Atlas*> atlas_pool; // 图集池
    std::unordered_map<std::string, IMAGE*> image_pool; // 图片池
};
