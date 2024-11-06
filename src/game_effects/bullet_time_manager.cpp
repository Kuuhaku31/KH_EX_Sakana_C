
// bullet_time_manager.cpp

#include "bullet_time_manager.h"

#include <graphics.h>

BulletTimeManager* BulletTimeManager::manager = nullptr;

BulletTimeManager*
BulletTimeManager::instance()
{
    if(!manager) manager = new BulletTimeManager();
    return manager;
}

void
BulletTimeManager::post_process()
{
    DWORD* buffer      = GetImageBuffer(nullptr);
    int    buffer_size = getwidth() * getheight();
    for(int i = 0; i < buffer_size; i++)
    {
        DWORD color = buffer[i];

        BYTE r = GetRValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress);
        BYTE g = GetGValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress);
        BYTE b = GetBValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress);

        buffer[i] = RGB(r, g, b) | 0xFF000000;
    }
}

void
BulletTimeManager::set_status(Status status)
{
    this->status = status;
}

float
BulletTimeManager::on_update(float delta)
{
    bool flag = false;
    if(status == Status::Entering)
    {
        if(bullet_time_energy > 0.0f)
        {
            // 如果有能量
            bullet_time_energy -= delta;
            if(bullet_time_energy < 0.0f) bullet_time_energy = 0.0f;
            flag = true;
        }
    }

    float delta_progress = SPEED_PROGRESS * delta;
    progress += delta_progress * (flag ? 1 : -1);

    if(progress < 0.0f) progress = 0.0f;
    if(progress > 1.0f) progress = 1.0f;

    delta *= lerp(1.0f, DST_DELTA_FACTOR, progress);

    return delta;
}

void
BulletTimeManager::add_energy(float energy)
{
    bullet_time_energy += energy;
    if(bullet_time_energy > ENERGY_MAX) bullet_time_energy = ENERGY_MAX;
}

BulletTimeManager::BulletTimeManager()  = default;
BulletTimeManager::~BulletTimeManager() = default;
