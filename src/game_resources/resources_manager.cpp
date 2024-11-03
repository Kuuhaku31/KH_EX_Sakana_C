
// resource_manager.cpp

#include "resources_manager.h"

#define PATH_TO_RESOURCES "../"

// 图片资源信息
struct ImageResInfo
{
    std::string id;
    LPCTSTR     path;
};

// 图集资源信息
struct AtlasResInfo
{
    std::string id;
    LPCTSTR     path;
    int         num_frame;
};

// 图片资源列表
static const std::vector<ImageResInfo> image_res_list = {

    { "player_attack_right", _T(R"(..\resources\player\attack.png)") },
    { "player_dead_right", _T(R"(..\resources\player\dead.png)") },
    { "player_fall_right", _T(R"(..\resources\player\fall.png)") },
    { "player_idle_right", _T(R"(..\resources\player\idle.png)") },
    { "player_jump_right", _T(R"(..\resources\player\jump.png)") },
    { "player_run_right", _T(R"(..\resources\player\run.png)") },
    { "player_roll_right", _T(R"(..\resources\player\roll.png)") },

    { "player_vfx_attack_down", _T(R"(..\resources\player\vfx_attack_down.png)") },
    { "player_vfx_attack_left", _T(R"(..\resources\player\vfx_attack_left.png)") },
    { "player_vfx_attack_right", _T(R"(..\resources\player\vfx_attack_right.png)") },
    { "player_vfx_attack_up", _T(R"(..\resources\player\vfx_attack_up.png)") },
    { "player_vfx_jump", _T(R"(..\resources\player\vfx_jump.png)") },
    { "player_vfx_land", _T(R"(..\resources\player\vfx_land.png)") },

    { "background", _T(R"(..\resources\background.png)") },
    { "ui_heart", _T(R"(..\resources\ui_heart.png)") },
};

// 图集资源列表
static const std ::vector<AtlasResInfo> atlas_info_list = {

    { "barb_break", _T(R"(..\resources\enemy\barb_break\%d.png)"), 3 },
    { "barb_loose", _T(R"(..\resources\enemy\barb_loose\%d.png)"), 5 },
    { "silk", _T(R"(..\resources\enemy\silk\%d.png)"), 9 },
    { "sword_left", _T(R"(..\resources\enemy\sword\%d.png)"), 3 },

    { "enemy_aim_left", _T(R"(..\resources\enemy\aim\%d.png)"), 9 },
    { "enemy_dash_in_air_left", _T(R"(..\resources\enemy\dash_in_air\%d.png)"), 2 },
    { "enemy_dash_on_floor_left", _T(R"(..\resources\enemy\dash_on_floor\%d.png)"), 2 },
    { "enemy_fall_left", _T(R"(..\resources\enemy\fall\%d.png)"), 4 },
    { "enemy_idle_left", _T(R"(..\resources\enemy\idle\%d.png)"), 6 },
    { "enemy_jump_left", _T(R"(..\resources\enemy\jump\%d.png)"), 8 },
    { "enemy_run_left", _T(R"(..\resources\enemy\run\%d.png)"), 8 },
    { "enemy_squat_left", _T(R"(..\resources\enemy\squat\%d.png)"), 10 },
    { "enemy_throw_barb_left", _T(R"(..\resources\enemy\throw_barb\%d.png)"), 8 },
    { "enemy_throw_silk_left", _T(R"(..\resources\enemy\throw_silk\%d.png)"), 17 },
    { "enemy_throw_sword_left", _T(R"(..\resources\enemy\throw_sword\%d.png)"), 16 },

    { "enemy_vfx_dash_in_air_left", _T(R"(..\resources\enemy\vfx_dash_in_air\%d.png)"), 5 },
    { "enemy_vfx_dash_on_floor_left", _T(R"(..\resources\enemy\vfx_dash_on_floor\%d.png)"), 6 },
};

static inline bool
check_image_valid(IMAGE* img)
{
    return GetImageBuffer(img);
}

ResourcesManager::ResourcesManager()  = default;
ResourcesManager::~ResourcesManager() = default;

ResourcesManager*
ResourcesManager::instance()
{
    if(manager == nullptr)
    {
        manager = new ResourcesManager();
    }

    return manager;
}

ResourcesManager* ResourcesManager::manager = nullptr;

void
ResourcesManager::load()
{
    // 加载图片资源
    for(const auto& info : image_res_list)
    {
        IMAGE* img = new IMAGE;
        loadimage(img, info.path);
        if(check_image_valid(img))
        {
            image_pool[info.id] = img;
        }
        else
        {
            throw info.path;
            delete img;
        }
    }

    // 加载图集资源
    for(const auto& info : atlas_info_list)
    {
        Atlas* atlas = new Atlas;
        atlas->load(info.path, info.num_frame);
        for(int i = 0; i < atlas->get_size(); i++)
        {
            IMAGE* img = atlas->get_image(i);
            if(!check_image_valid(img))
            {
                throw info.path;
                delete atlas;
                return;
            }
            atlas_pool[info.id] = atlas;
        }
    }

    // 反转图片
    flip_image("player_attack_right", "player_attack_left", 5);
    flip_image("player_dead_right", "player_dead_left", 6);
    flip_image("player_fall_right", "player_fall_left", 5);
    flip_image("player_idle_right", "player_idle_left", 5);
    flip_image("player_jump_right", "player_jump_left", 5);
    flip_image("player_run_right", "player_run_left", 10);
    flip_image("player_roll_right", "player_roll_left", 7);

    flip_atlas("sword_left", "sword_right");
    flip_atlas("enemy_aim_left", "enemy_aim_right");
    flip_atlas("enemy_dash_in_air_left", "enemy_dash_in_air_right");
    flip_atlas("enemy_dash_on_floor_left", "enemy_dash_on_floor_right");
    flip_atlas("enemy_fall_left", "enemy_fall_right");
    flip_atlas("enemy_idle_left", "enemy_idle_right");
    flip_atlas("enemy_jump_left", "enemy_jump_right");
    flip_atlas("enemy_run_left", "enemy_run_right");
    flip_atlas("enemy_squat_left", "enemy_squat_right");
    flip_atlas("enemy_throw_barb_left", "enemy_throw_barb_right");
    flip_atlas("enemy_throw_silk_left", "enemy_throw_silk_right");
    flip_atlas("enemy_throw_sword_left", "enemy_throw_sword_right");

    flip_atlas("enemy_vfx_dash_in_air_left", "enemy_vfx_dash_in_air_right");
    flip_atlas("enemy_vfx_dash_on_floor_left", "enemy_vfx_dash_on_floor_right");

    // 加载音频资源
    load_audio(_T(R"(..\resources\audio\bgm.mp3)"), _T("bgm"));
    load_audio(_T(R"(..\resources\audio\barb_break.mp3)"), _T("barb_break"));
    load_audio(_T(R"(..\resources\audio\bullet_time.mp3)"), _T("bullet_time"));

    load_audio(_T(R"(..\resources\audio\enemy_dash.mp3)"), _T("enemy_dash"));
    load_audio(_T(R"(..\resources\audio\enemy_run.mp3)"), _T("enemy_run"));
    load_audio(_T(R"(..\resources\audio\enemy_hurt_1.mp3)"), _T("enemy_hurt_1"));
    load_audio(_T(R"(..\resources\audio\enemy_hurt_2.mp3)"), _T("enemy_hurt_2"));
    load_audio(_T(R"(..\resources\audio\enemy_hurt_3.mp3)"), _T("enemy_hurt_3"));
    load_audio(_T(R"(..\resources\audio\enemy_throw_barbs.mp3)"), _T("enemy_throw_barbs"));
    load_audio(_T(R"(..\resources\audio\enemy_throw_silk.mp3)"), _T("enemy_throw_silk"));
    load_audio(_T(R"(..\resources\audio\enemy_throw_sword.mp3)"), _T("enemy_throw_sword"));

    load_audio(_T(R"(..\resources\audio\player_attack_1.mp3)"), _T("player_attack_1"));
    load_audio(_T(R"(..\resources\audio\player_attack_2.mp3)"), _T("player_attack_2"));
    load_audio(_T(R"(..\resources\audio\player_attack_3.mp3)"), _T("player_attack_3"));
    load_audio(_T(R"(..\resources\audio\player_dead.mp3)"), _T("player_dead"));
    load_audio(_T(R"(..\resources\audio\player_hurt.mp3)"), _T("player_hurt"));
    load_audio(_T(R"(..\resources\audio\player_jump.mp3)"), _T("player_jump"));
    load_audio(_T(R"(..\resources\audio\player_land.mp3)"), _T("player_land"));
    load_audio(_T(R"(..\resources\audio\player_roll.mp3)"), _T("player_roll"));
    load_audio(_T(R"(..\resources\audio\player_run.mp3)"), _T("player_run"));
}

IMAGE*
ResourcesManager::find_image(const std::string& id) const
{
    const auto it = image_pool.find(id);
    if(it != image_pool.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Atlas*
ResourcesManager::find_atlas(const std::string& id) const
{
    const auto it = atlas_pool.find(id);
    if(it != atlas_pool.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

// 反转图片
void
ResourcesManager::flip_image(IMAGE* src_img, IMAGE* dst_img, int num)
{
    int w       = src_img->getwidth();
    int h       = src_img->getheight();
    int w_frame = w / num;

    Resize(dst_img, w, h);
    DWORD* src_buf = GetImageBuffer(src_img);
    DWORD* dst_buf = GetImageBuffer(dst_img);

    for(int i = 0; i < num; i++)
    {
        int x_left = i * w_frame;
        int x_righ = x_left + w_frame;
        for(int y = 0; y < h; y++)
        {
            for(int x = x_left; x < x_righ; x++)
            {
                int idx_src = y * w + x;
                int idx_dst = y * w + x_righ - (x - x_left);

                dst_buf[idx_dst] = src_buf[idx_src];
            }
        }
    }
}

void
ResourcesManager::flip_image(const std::string& src_id, const std::string& dst_id, int num)
{
    IMAGE* src_img = image_pool[src_id];
    IMAGE* dst_img = new IMAGE;

    flip_image(src_img, dst_img, num);

    image_pool[dst_id] = dst_img;
}

// 反转图集
void
ResourcesManager::flip_atlas(const std::string& src_id, const std::string& dst_id)
{
    Atlas* src_atlas = atlas_pool[src_id];
    Atlas* dst_atlas = new Atlas;

    for(int i = 0; i < src_atlas->get_size(); i++)
    {
        IMAGE img_flipped;
        flip_image(src_atlas->get_image(i), &img_flipped);
        dst_atlas->add_image(img_flipped);
    }

    atlas_pool[dst_id] = dst_atlas;
}
