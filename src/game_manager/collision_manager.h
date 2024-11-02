
// collision_manager.h

#pragma once

#include "collision_box.h"

#include <vector>

class CollisionManager
{
public:
    static CollisionManager* instance();

    CollisionBox* create_collision_box();
    void          destroy_collision_box(CollisionBox* box);

    void process_collide();
    void on_debug_render();

private:
    static CollisionManager*   manager;
    std::vector<CollisionBox*> collision_box_list;

private:
    CollisionManager();
    ~CollisionManager();
};
