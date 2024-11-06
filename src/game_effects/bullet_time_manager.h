
// bullet_time_manager.h

#pragma once

class BulletTimeManager
{
public:
    enum class Status
    {
        Entering,
        Exiting,
    };

public:
    static BulletTimeManager* instance(); // 单例

    void  post_process();            // 后处理
    void  set_status(Status status); // 设置状态
    float on_update(float delta);    // 更新

    void  add_energy(float energy);
    float get_energy() { return bullet_time_energy; }
    float get_max_energy() { return ENERGY_MAX; }

private:
    static BulletTimeManager* manager;

    float bullet_time_energy = 0.0f; // 子弹时间能量

    float  progress = 0; // 0~1 之间的值，表示进度
    Status status   = Status::Exiting;

    const float ENERGY_MAX       = 3.0f;  // 子弹时间能量最大值（单位：秒）
    const float SPEED_PROGRESS   = 2.0f;  // 进度变化速度
    const float DST_DELTA_FACTOR = 0.35f; // 完全进入子弹时间时的delta时间缩放
    const float DST_COLOR_FACTOR = 0.35f; // 完全进入子弹时间时的颜色改变比例

private:
    BulletTimeManager();
    ~BulletTimeManager();

    // 线性插值
    float lerp(float a, float b, float t) { return a + (b - a) * t; }
};
