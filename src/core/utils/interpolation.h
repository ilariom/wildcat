#ifndef WKT_INTERPOLATION_H
#define WKT_INTERPOLATION_H

namespace wkt
{

class Interpolator
{
public:
    Interpolator() = default;
    Interpolator(
        float start,
        float end,
        float duration,
        float delay = 0
    )
        : start(start)
        , end(end)
        , duration(duration)
        , delay(delay)
    { }

public:
    inline float update(float dt);
    bool hasEnded() const { return this->t >= this->delay + this->duration; }

    float getDelay() const { return this->delay; }
    float getDuration() const { return this->duration; }
    float getStart() const { return this->start; }
    float getEnd() const { return this->end; }
    float getTimeElapsed() const { return this->t; }

private:
    float start, end, duration, delay;
    float t = 0;
};

inline float Interpolator::update(float dt)
{
    if (this->t < this->delay)
    {
        return this->start;
    }

    this->t += dt;

    return this->start + this->t * (this->end - this->start) / (this->delay + this->duration);
}

}

#endif