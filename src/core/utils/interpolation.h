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
    inline float update();
    bool hasEnded() const { return this->t >= this->delay + this->duration; }

    float getDelay() const { return this->delay; }
    float getDuration() const { return this->duration; }
    float getStart() const { return this->start; }
    float getEnd() const { return this->end; }
    float getTimeElapsed() const { return this->t; }

    void setResolution(float resolution) { this->resolution = resolution; }
    float getResolution() const { return this->resolution; }

private:
    float start, end, duration, delay;
    float t = 0;
    float resolution = .016667f;
};

inline float Interpolator::update()
{
    if (this->t < this->delay)
    {
        return this->start;
    }

    this->t += this->resolution;

    return this->start + (this->t - this->delay) * (this->end - this->start) / this->duration;
}

}

#endif