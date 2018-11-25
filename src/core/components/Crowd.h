#ifndef WKT_CROWD_H
#define WKT_CROWD_H

#include "ecs/Drawable.h"
#include "components/Sprite.h"
#include "components/Transform.h"
#include "graphics/Director.h"
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

    void draw(const wkt::gph::Director&, const Transform&) override;

    void setOpacity(uint8_t);
    void setColor(const wkt::gph::Color&);
    uint8_t getOpacity() const { return this->opacity; }
    const wkt::gph::Color& getColor() const { return this->color; }
    
private:
    std::unordered_map<SpectatorID, Spectator> spectators;
    uint8_t opacity = 255;
    wkt::gph::Color color = wkt::gph::colors::WHITE;
};

REGISTER_DRAWABLE(Crowd, -12);

}}

#endif