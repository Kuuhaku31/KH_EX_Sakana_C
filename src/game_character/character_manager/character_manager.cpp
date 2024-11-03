
// character_manager.cpp

#include "character_manager.h"

CharacterManager* CharacterManager::manager = nullptr;

CharacterManager*
CharacterManager::instance()
{
    if(manager == nullptr)
    {
        manager = new CharacterManager();
    }

    return manager;
}

void
CharacterManager::on_input(const ExMessage& msg)
{
    player->on_input(msg);
}

void
CharacterManager::on_update(float delta)
{
    player->on_update(delta);
    enemy->on_update(delta);
}

void
CharacterManager::on_render()
{
    enemy->on_render();
    BulletTimeManager::instance()->post_process(); // 后处理
    player->on_render();
}

CharacterManager::CharacterManager()
{
    player = new Player();
    enemy  = new Enemy();
}

CharacterManager::~CharacterManager()
{
    delete player;
    delete enemy;
}
