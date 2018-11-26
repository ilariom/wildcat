#ifndef _WKT_DRAWABLE_H
#define _WKT_DRAWABLE_H

#include "ecs/Component.h"
#include "components/Transform.h"
#include "graphics/Director.h"

namespace wkt {
namespace ecs
{

class Drawable : public Component
{
public:
    virtual void draw(const wkt::gph::Director&, const wkt::components::Transform&) = 0;
    void setVisible(bool enable) { this->visible = enable; }
    bool isVisible() const { return this->visible; }
    
private:
    bool visible = true;
};

}}

#endif