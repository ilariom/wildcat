#ifndef WKT_CROWD_H
#define WKT_CROWD_H

#include "ecs/Drawable.h"
#include "components/Sprite.h"
#include "components/Transform.h"
#include <string>
#include <unordered_map>

namespace wkt {
namespace components
{

using Spectator = std::pair<Sprite, Transform>;

Spectator makeSpectator(const std::string& filename, const Transform&);
Spectator makeSpectator(const Sprite&, const Transform&);
Sprite& sprite(const Spectator&);
Transform& transform(const Spectator&);

class Crowd : public wkt::ecs::Drawable
{
public:
    using SpectatorID = uint32_t;

public:
    SpectatorID insert(const Spectator& s);
    SpectatorID emplace(const std::string& filename, const Transform& t);
    void erase(SpectatorID id);

    void draw(s2x::Renderer&, const Transform&) override;

private:
    std::unordered_map<SpectatorID, Spectator> spectators;
};

REGISTER_DRAWABLE(Crowd, -12);

}}

#endif