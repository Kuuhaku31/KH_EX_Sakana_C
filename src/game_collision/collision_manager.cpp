
// collision_manager.cpp

#include "collision_manager.h"
#include <graphics.h>

CollisionManager::CollisionManager()  = default;
CollisionManager::~CollisionManager() = default;

CollisionManager* CollisionManager::manager = nullptr;

// 获取单例
CollisionManager*
CollisionManager::instance()
{
    if(manager == nullptr)
    {
        manager = new CollisionManager();
    }
    return manager;
}

// 创建碰撞盒
CollisionBox*
CollisionManager::create_collision_box()
{
    CollisionBox* box = new CollisionBox;
    collision_box_list.push_back(box);
    return box;
}

// 销毁碰撞盒
void
CollisionManager::destroy_collision_box(CollisionBox* collision_box)
{
    // 从列表中移除碰撞盒
    collision_box_list.erase(
        std::remove(collision_box_list.begin(),
                    collision_box_list.end(),
                    collision_box),
        collision_box_list.end());

    // 释放碰撞盒内存
    delete collision_box;
}

// 处理碰撞
void
CollisionManager::process_collide()
{
    // 依次取出所有碰撞箱
    for(CollisionBox* box_src : collision_box_list)
    {
        // 如果未启用碰撞或者检测的碰撞层级为None
        if(!box_src->enable || box_src->layer_dst == CollisionLayer::None)
        {
            continue;
        }

        // 依次检测所有碰撞箱
        for(CollisionBox* box_dst : collision_box_list)
        {
            // 如果未启用碰撞或者本身碰撞层级和box_src不同，或者两个碰撞箱是同一个
            if(!box_dst->enable ||
               box_dst->layer_src != box_src->layer_dst ||
               box_src == box_dst)
            {
                continue;
            }

            // 碰撞检测逻辑
            bool is_collide_x = false;
            bool is_collide_y = false;

            // 计算碰撞盒的边界
            int box_src_left   = box_src->position.vx - box_src->size.vx / 2;
            int box_src_right  = box_src->position.vx + box_src->size.vx / 2;
            int box_src_top    = box_src->position.vy - box_src->size.vy / 2;
            int box_src_bottom = box_src->position.vy + box_src->size.vy / 2;

            int box_dst_left   = box_dst->position.vx - box_dst->size.vx / 2;
            int box_dst_right  = box_dst->position.vx + box_dst->size.vx / 2;
            int box_dst_top    = box_dst->position.vy - box_dst->size.vy / 2;
            int box_dst_bottom = box_dst->position.vy + box_dst->size.vy / 2;

            // 检查X轴方向的碰撞
            is_collide_x = (box_src_right >= box_dst_left) && (box_src_left <= box_dst_right);

            // 检查Y轴方向的碰撞
            is_collide_y = (box_src_bottom >= box_dst_top) && (box_src_top <= box_dst_bottom);

            // 如果碰撞
            if(is_collide_x && is_collide_y)
            {
                // 调用碰撞回调函数
                if(box_dst->on_collision)
                {
                    box_dst->on_collision();
                }
            }
        }
    }
}
