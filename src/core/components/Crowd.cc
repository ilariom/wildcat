#include "Crowd.h"
#include <limits>

namespace
{
    ::wkt::components::Crowd::SpectatorID uid = 0;
}

namespace wkt {
namespace components
{

const Crowd::SpectatorID Crowd::NULL_ID = std::numeric_limits<Crowd::SpectatorID>::max();

Spectator makeSpectator(const std::string& filename, const Transform& t)
{
    return std::make_pair(Sprite(filename), t);
}

Spectator makeSpectator(const Sprite& s, const Transform& t)
{
    return std::make_pair(s, t);
}

Sprite& sprite(Spectator& s)
{
    return s.first;
}

Transform& transform(Spectator& s)
{
    return s.second;
}

const Sprite& sprite(const Spectator& s)
{
    return s.first;
}

const Transform& transform(const Spectator& s)
{
    return s.second;
}

Crowd::SpectatorID Crowd::insert(const Spectator& s)
{
    SpectatorID sid = uid++;
    this->spectators.emplace_back(s);

    return sid;
}

Crowd::SpectatorID Crowd::emplace(const std::string& filename, const Transform& t)
{
    SpectatorID sid = uid++;

    this->spectators.emplace_back(
        makeSpectator(filename, t)
    );

    return sid;
}

void Crowd::erase(SpectatorID id)
{
    this->spectators.erase(this->spectators.begin() + id);
}

void Crowd::draw(const wkt::gph::Director& d, const Transform& t)
{
    for (SpectatorID id = 0; id < this->spectators.size(); ++id)
    {
        Spectator& sp = this->spectators[id];
        Transform& localTransform = transform(sp);
        Sprite& localSprite = sprite(sp);

        if (flat() || id == root())
            localTransform.setParentCoordinates(t.getWorldCoordinates());

        localSprite.draw(d, localTransform);
    }
}

void Crowd::setOpacity(uint8_t opacity)
{
    this->opacity = opacity;

    for (auto& p : this->spectators)
        sprite(p).setOpacity(opacity);
}

void Crowd::setColor(const wkt::gph::Color& color)
{
    this->color = color;

    for (auto& p : this->spectators)
        sprite(p).setColor(color);
}

}}