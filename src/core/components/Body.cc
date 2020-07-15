#include "Body.h"
#include "components/Transform.h"

namespace wkt {
namespace components
{

wkt::math::vec2 Body::Box::topLeft() const
{
    return { x0, y0 };
}

wkt::math::vec2 Body::Box::topRight() const
{
    return { x1, y0 };
}

wkt::math::vec2 Body::Box::bottomLeft() const
{
    return { x0, y1 };
}

wkt::math::vec2 Body::Box::bottomRight() const
{
    return { x1, y1 };
}

float Body::Box::width() const
{
    return x1 - x0;
}

float Body::Box::height() const
{
    return y1 - y0;
}

wkt::math::Size Body::Box::size() const
{
    return {
        width(),
        height()
    };
}

bool Body::Box::contains(const wkt::math::vec2& v, const wkt::math::vec2& pCoords) const
{
    wkt::math::vec2 w = v - pCoords;

    return 0 <= v.x && v.x <= width()
        && 0 <= v.y && v.y <= height()
    ;
}

bool Body::hit(const wkt::math::vec2& v) const
{
    bool res = false;

    for (auto& box : getBoxes())
        res = box.contains(v, this->transform.getParentCoordinates().position) || res;

    return res;    
}

bool Body::hit(const Box& b) const
{
    return hit(b.topLeft())
        || hit(b.topRight())
        || hit(b.bottomLeft())
        || hit(b.bottomRight())
    ;
}

void Body::push(const wkt::math::vec2& ds) const
{
    this->transform.addPosition(
        (1.f / this->conf.mass) * ds
    );
}

void Body::torque(float angle) const
{
    this->transform.addRotation(
        (1.f / this->conf.mass) * this->conf.torque * angle
    );
}

bool Body::unique() const
{
    return false;
}

}}