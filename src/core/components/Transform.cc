#include "Transform.h"
#include "math/numerical.h"

namespace wkt
{
namespace components
{

Coords Transform::getWorldCoordinates() const 
{
    Coords o;
    float angle = wkt::math::toRadians(this->parent.rotation);
    float s = sinf(angle);
    float c = cosf(angle);

    float x = this->local.position.x;
    float y = -this->local.position.y;
    o.position.x = x * c * this->parent.scaleX + y * s;
    o.position.y = x * s + y * -c * this->parent.scaleY;
    o.position += this->parent.position;
    o.rotation = this->parent.rotation + this->local.rotation;
    o.rotationAnchor = this->local.rotationAnchor;
    o.scaleX = this->parent.scaleX * this->local.scaleX;
    o.scaleY = this->parent.scaleY * this->local.scaleY;

    return o; 
}

Coords Transform::getLocalCoordinates(const Coords& coords) const
{
    Coords c;
    c.rotation = coords.rotation - this->parent.rotation;
    c.scaleX = coords.scaleX / this->parent.scaleX;
    c.scaleY = coords.scaleY / this->parent.scaleY;
    c.position -= this->parent.position;

    return c;
}

Coords& Coords::operator*=(const Coords& other)
{
    this->position += other.position;
    this->rotationAnchor += other.rotationAnchor;
    this->rotation += other.rotation;
    this->scaleX *= other.scaleX;
    this->scaleY *= other.scaleY;

    return *this;
}

Coords operator*(const Coords& a, const Coords& b)
{
    Coords c = a;
    c *= b;
    return c;
}

}}