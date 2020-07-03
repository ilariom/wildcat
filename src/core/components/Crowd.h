#ifndef WKT_CROWD_H
#define WKT_CROWD_H

#include "ecs/Drawable.h"
#include "components/Sprite.h"
#include "components/Transform.h"
#include "graphics/Director.h"
#include <string>
#include <vector>

namespace wkt {
namespace components
{

using Spectator = std::pair<Sprite, Transform>;

Spectator makeSpectator(const std::string& filename, const Transform&);
Spectator makeSpectator(const Sprite&, const Transform&);
Sprite& sprite(Spectator&);
Transform& transform(Spectator&);
const Sprite& sprite(const Spectator&);
const Transform& transform(const Spectator&);

class Crowd : public wkt::ecs::Drawable
{
public:
    using SpectatorID = size_t;
    static const SpectatorID NULL_ID;

public:
    SpectatorID insert(const Spectator& s);
    SpectatorID emplace(const std::string& filename, const Transform& t);
    void erase(SpectatorID id);

    void draw(const wkt::gph::Director&, const Transform&) override;

    void setOpacity(uint8_t);
    void setColor(const wkt::gph::Color&);
    uint8_t getOpacity() const { return this->opacity; }
    const wkt::gph::Color& getColor() const { return this->color; }

    bool flat() const { return this->flatted; }
    void setFlat(bool enable) { this->flatted = enable; }

    void setRoot(SpectatorID id) { this->rootId = id; }
    SpectatorID root() const { return this->rootId; }
    
private:
    std::vector<Spectator> spectators;
    uint8_t opacity = 255;
    wkt::gph::Color color = wkt::gph::colors::WHITE;
    SpectatorID rootId = NULL_ID;
    bool flatted = true;
};

REGISTER_DRAWABLE(Crowd, -12);

}}

#endif