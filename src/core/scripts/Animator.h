#ifndef WKT_ANIMATOR_H
#define WKT_ANIMATOR_H

#include "components/Script.h"
#include "components/Transform.h"
#include "components/Crowd.h"
#include "graphics/Flipbook.h"
#include "utils/interpolation.h"
#include <string>
#include <array>
#include <vector>
#include <utility>
#include <functional>

namespace wkt {
namespace scripts
{

class AnimatorBase : public wkt::components::Script
{
public:
    void onMessage(const std::string& msg, const wkt::ecs::Entity& sender) override { }

    inline void start();
    inline void stop();
    bool isRunning() const { return isUpdateScheduled(); }

    inline void complete();

public:
    std::function<void()> startListener = nullptr;
    std::function<void()> stopListener = nullptr;
    std::function<void()> completeListener = nullptr;
};

inline void AnimatorBase::start() 
{
    scheduleUpdate();

    if (this->startListener)
        this->startListener();
}

inline void AnimatorBase::stop() 
{ 
    unscheduleUpdate();

    if (this->stopListener)
        this->stopListener();
}

inline void AnimatorBase::complete() 
{
    if (this->completeListener)
        this->completeListener();

    stop();
}

class EventAnimatorBase : public AnimatorBase
{
protected:
    inline void sendEvent(const std::string& ev);

public:
    std::function<void(const std::string&)> eventListener = nullptr;
};

inline void EventAnimatorBase::sendEvent(const std::string& ev)
{
    if (this->eventListener)
        this->eventListener(ev);
}

struct Keyframe
{
    float duration;
    wkt::components::Coords sample;
    std::string event;

    Keyframe(float duration, const wkt::components::Coords& sample, const std::string& event = "")
        : duration(duration), sample(sample), event(event)
    { }

    Keyframe() = default;
};

class CrowdAnimator;

class Animator : public EventAnimatorBase
{
    friend CrowdAnimator;

public:
    Animator(wkt::components::Transform& transform, bool loop = false)
        : transform(transform), loop(loop) 
    { }

public:
    void init() override;
    void update(duration dt) override;

    std::vector<Keyframe>& getKeyframes() { return this->keyframes; }
    Keyframe& currentKeyframe() { return this->keyframes[this->k - 1]; }

    void rewind();

private:
    bool setupNext();

private:
    std::vector<Keyframe> keyframes;
    std::array<wkt::Interpolator, 7> interpolators;
    wkt::components::Transform& transform;
    std::string event;
    size_t k = 0;
    bool loop;
};

class FlipbookAnimator : public AnimatorBase
{
public:
    FlipbookAnimator(wkt::components::Sprite& sprite)
        : sprite(sprite)
    { }

    FlipbookAnimator(wkt::components::Sprite& sprite, wkt::gph::FlipbookChannels&& fc)
        : sprite(sprite), fc(fc)
    { }

public:
    void init() override { }
    void update(duration dt) override;

    void rewind();
    wkt::gph::FlipbookChannels& getFlipbookChannels() { return this->fc; }

private:
    wkt::gph::FlipbookChannels fc;
    wkt::components::Sprite& sprite;
};

class CrowdAnimator : public EventAnimatorBase
{
public:
    CrowdAnimator(wkt::components::Crowd& crowd, bool loop);

public:
    void init() override;
    void update(duration dt) override;

    void rewind();

    std::vector<Keyframe>& getKeyframes(wkt::components::Crowd::SpectatorID id) 
    { return this->animators[id].getKeyframes(); }

private:
    std::vector<Animator> animators;
    wkt::components::Crowd& crowd;
};

}

namespace components
{
REGISTER_SCRIPT(wkt::scripts::Animator);
REGISTER_SCRIPT(wkt::scripts::FlipbookAnimator);
REGISTER_SCRIPT(wkt::scripts::CrowdAnimator);
}

}


#endif