
// collision_layer.h

#pragma once

enum class CollisionLayer
{
    None         = 0,
    Player       = 1,
    Enemy        = 2,
    PlayerBullet = 4,
    EnemyBullet  = 8,
    PlayerVfx    = 16,
    EnemyVfx     = 32,
    All          = 0xFFFF,
};
