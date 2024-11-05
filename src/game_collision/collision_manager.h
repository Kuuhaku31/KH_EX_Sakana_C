
// collision_manager.h

#pragma once

#include "collision_box.h"

#include <algorithm>
#include <graphics.h>

class CollisionManager
{
public:
    static CollisionManager* instance();

    CollisionBox* create_collision_box();
    void          destroy_collision_box(CollisionBox* box);

    void                              process_collide();
    const std::vector<CollisionBox*>& get_collision_box_list() const
    {
        return collision_box_list;
    }

private:
    static CollisionManager*   manager;
    std::vector<CollisionBox*> collision_box_list;

private:
    CollisionManager();
    ~CollisionManager();
};
