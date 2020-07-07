#include "Animator.h"

namespace wkt {
namespace scripts
{

void Animator::init()
{
    setupNext();
}

bool Animator::setupNext()
{
    if (this->k >= this->keyframes.size())
    {
        if (this->loop)
            this->k = 0;
        else
            return false;
    }
    
    auto& next = this->keyframes[this->k++];
    const auto& coords = this->transform.getCoordinates();
    const auto& nextCoords = next.sample;
    float duration = next.duration;

    this->interpolators[0] = wkt::Interpolator(coords.position.x, nextCoords.position.x, duration);
    this->interpolators[1] = wkt::Interpolator(coords.position.y, nextCoords.position.y, duration);
    this->interpolators[2] = wkt::Interpolator(coords.rotationAnchor.x, nextCoords.rotationAnchor.x, duration);
    this->interpolators[3] = wkt::Interpolator(coords.rotationAnchor.y, nextCoords.rotationAnchor.y, duration);
    this->interpolators[4] = wkt::Interpolator(coords.rotation, nextCoords.rotation, duration);
    this->interpolators[5] = wkt::Interpolator(coords.scaleX, nextCoords.scaleX, duration);
    this->interpolators[6] = wkt::Interpolator(coords.scaleY, nextCoords.scaleY, duration);

    if (!next.event.empty())
        sendEvent(next.event);

    return true;
}

void Animator::update(duration)
{
    if (this->interpolators[0].hasEnded() && !setupNext())
    {
        complete();
        return;
    }

    wkt::components::Coords nextCoords;

    nextCoords.position.x = this->interpolators[0].update();
    nextCoords.position.y = this->interpolators[1].update();
    nextCoords.rotationAnchor.x = this->interpolators[2].update();
    nextCoords.rotationAnchor.y = this->interpolators[3].update();
    nextCoords.rotation = this->interpolators[4].update();
    nextCoords.scaleX = this->interpolators[5].update();
    nextCoords.scaleY = this->interpolators[6].update();

    this->transform.setCoords(nextCoords);
}

void Animator::rewind()
{
    this->k = 0;
    setupNext();
}

void FlipbookAnimator::update(duration dt)
{
    if (this->fc.hasNext())
        this->sprite.crop(
            this->fc.next().rect
        );
    else
        complete();
}

CrowdAnimator::CrowdAnimator(wkt::components::Crowd& crowd) 
    : crowd(crowd) 
{
    for (auto k = 0; k < crowd.size(); ++k)
    {
        this->animators.push_back(crowd[k].second);
        this->animators.back().eventListener = [this] (const std::string& ev) {
            sendEvent(ev);
        };
    }
}

void CrowdAnimator::init()
{
    for (auto& animator : this->animators)
        animator.init();
}

void CrowdAnimator::update(duration dt)
{
    bool hasEnded = true;

    for (auto& animator : this->animators)
    {
        animator.update(dt);
        hasEnded = animator.interpolators[0].hasEnded() && hasEnded;
    }

    if (hasEnded)
        complete();
}

void CrowdAnimator::rewind()
{
    for (auto& animator : this->animators)
        animator.rewind();
}

}}