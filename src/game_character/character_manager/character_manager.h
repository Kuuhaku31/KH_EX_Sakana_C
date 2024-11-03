
// character_manager.h

#pragma once

#include "enemy.h"
#include "player.h"

class CharacterManager
{
public:
    static CharacterManager* instance();

    Character*
    get_enemy()
    {
        return enemy;
    }

    Character*
    get_player()
    {
        return player;
    }

    void on_input(const ExMessage& msg); // 操控角色
    void on_update(float delta);         // 更新角色
    void on_render();                    // 渲染角色

private:
    static CharacterManager* manager;

    Character* enemy  = nullptr;
    Character* player = nullptr;

private:
    CharacterManager();
    ~CharacterManager();
};
