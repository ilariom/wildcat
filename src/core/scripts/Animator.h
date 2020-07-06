#ifndef WKT_ANIMATOR_H
#define WKT_ANIMATOR_H

#include "components/Script.h"
#include "components/Transform.h"
#include "components/Crowd.h"
#include "graphics/Flipbook.h"
#include "utils/interpolation.h"
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

    std::function<void()> startListener = nullptr;
    std::function<void()> stopListener = nullptr;
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

using Keyframe = std::pair<float, wkt::components::Coords>;
class CrowdAnimator;

class Animator : public AnimatorBase
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

private:
    bool setupNext();

private:
    std::vector<Keyframe> keyframes;
    std::array<wkt::Interpolator, 7> interpolators;
    wkt::components::Transform& transform;
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

    wkt::gph::FlipbookChannels& getFlipbookChannels() { return this->fc; }

private:
    wkt::gph::FlipbookChannels fc;
    wkt::components::Sprite& sprite;
};

class CrowdAnimator : public AnimatorBase
{
public:
    CrowdAnimator(wkt::components::Crowd& crowd) 
        : crowd(crowd) 
    {
        for (auto k = 0; k < crowd.size(); ++k)
            this->animators.push_back(crowd[k].second);
    }

public:
    void init() override;
    void update(duration dt) override;

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