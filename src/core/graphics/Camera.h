#ifndef WKT_CAMERA_H
#define WKT_CAMERA_H

#include "math/wktmath.h"
#include "components/Transform.h"

namespace wkt {
namespace gph
{

class Camera
{
public:
    void setPosition(const wkt::math::vec2& pos) { this->pos = pos; this->dirty = true; }
    void setSize(const wkt::math::Size& sz) { this->sz = sz; this->dirty = true; }
    void setRotation(float rot) { this->rot = rot; }

    const wkt::math::vec2& position() const { return this->pos; }
    const wkt::math::Size& size() const { return this->sz; }
    float rotation() const { return this->rot; }
    inline const wkt::math::Rect& getBoundingBox() const;

    inline wkt::components::Transform getScreenCoordinates(const wkt::components::Transform& t) const;

private:
    wkt::math::vec2 pos {0, 0};
    wkt::math::Size sz;
    mutable wkt::math::Rect boundingBox;
    mutable bool dirty = false;
    float rot = 0;
};

inline wkt::components::Transform Camera::getScreenCoordinates(const wkt::components::Transform& t) const
{
    auto obj = t.getWorldCoordinates();
    wkt::components::Transform scrCoords;
    scrCoords.setPosition(obj.position - position());
    scrCoords.setRotation(obj.rotation - rotation());

    return scrCoords;
}

inline const wkt::math::Rect& Camera::getBoundingBox() const
{
    if(this->dirty)
    {
        this->boundingBox = {
            position(),
            size()
        };

        this->dirty = false;
    }

    return this->boundingBox;
}

}}

#endif