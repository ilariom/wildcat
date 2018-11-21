#include "Crowd.h"

namespace
{
    ::wkt::components::Crowd::SpectatorID uid = 0;
}

namespace wkt {
namespace components
{

Spectator makeSpectator(const std::string& filename, const Transform& t)
{
    return std::make_pair(Sprite(filename), t);
}

Spectator makeSpectator(const Sprite& s, const Transform& t)
{
    return std::make_pair(s, t);
}

Sprite& sprite(const Spectator& s)
{
    return const_cast<Spectator&>(s).first;
}

Transform& transform(const Spectator& s)
{
    return const_cast<Spectator&>(s).second;
}

Crowd::SpectatorID Crowd::insert(const Spectator& s)
{
    SpectatorID sid = uid++;
    this->spectators.emplace(sid, s);
    return sid;
}

Crowd::SpectatorID Crowd::emplace(const std::string& filename, const Transform& t)
{
    SpectatorID sid = uid++;
    this->spectators.emplace(sid, std::move(makeSpectator(filename, t)));
    return sid;
}

void Crowd::erase(SpectatorID id)
{
    this->spectators.erase(id);
}

void Crowd::draw(s2x::Renderer& r, const Transform& t)
{
    for(auto& p : this->spectators)
    {
        Spectator& sp = p.second;
        Transform& localTransform = transform(sp);
        Sprite& localSprite = sprite(sp);
        localTransform.setParentCoordinates(t.getWorldCoordinates());
        localSprite.draw(r, localTransform);
    }
}

}}