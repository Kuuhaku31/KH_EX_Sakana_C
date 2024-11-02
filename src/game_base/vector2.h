
// vector2.h

#pragma once

#include <cmath>

class Vector2
{
public:
    float vx = 0;
    float vy = 0;

public:
    Vector2() = default;
    Vector2(float x, float y)
        : vx(x)
        , vy(y)
    {
    }

    ~Vector2() = default;

    Vector2
    operator+(const Vector2& v) const
    {
        return Vector2(vx + v.vx, vy + v.vy);
    }

    void
    operator+=(const Vector2& v)
    {
        vx += v.vx;
        vy += v.vy;
    }

    Vector2
    operator-(const Vector2& v) const
    {
        return Vector2(vx - v.vx, vy - v.vy);
    }

    void
    operator-=(const Vector2& v)
    {
        vx -= v.vx;
        vy -= v.vy;
    }

    Vector2
    operator*(float f) const
    {
        return Vector2(vx * f, vy * f);
    }

    void
    operator*=(float f)
    {
        vx *= f;
        vy *= f;
    }

    float
    module() const
    {
        return sqrt(vx * vx + vy * vy);
    }

    Vector2
    tounit() const
    {
        float m = module();

        if(m == 0)
        {
            return Vector2(0, 0);
        }
        else
        {
            return Vector2(vx / m, vy / m);
        }
    }
};
