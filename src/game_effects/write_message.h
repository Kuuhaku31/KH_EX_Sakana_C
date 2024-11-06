
// write_message.h

#pragma once

enum class TxtType
{
    DEAD,
    DEAD_2,
    WIN
};

void
write_message(TxtType type, float progress);

class WriteMessage
{
public:
    static WriteMessage* instance()
    {
        if(!ins)
        {
            WriteMessage::ins = new WriteMessage();
        }
        return WriteMessage::ins;
    }

    void set_message(TxtType t)
    {
        type = t;
    }

    void post_process()
    {
        write_message(type, progress);
    }

    void on_update(float delta)
    {
        float delta_progress = SPEED_PROGRESS * delta;
        progress += delta_progress;

        if(progress < 0.0f) progress = 0.0f;
        if(progress > 1.0f) progress = 1.0f;
    }

    bool is_done()
    {
        return progress >= 1.0f;
    }

private:
    WriteMessage()  = default;
    ~WriteMessage() = default;

private:
    static WriteMessage* ins;

    TxtType     type;
    float       progress;              // 0~1 之间的值，表示进度
    const float SPEED_PROGRESS = 1.0f; // 进度变化速度
};
