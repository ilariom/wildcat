#include "Transform.h"

namespace wkt
{
namespace components
{

namespace {
    constexpr float toDegree(float angleinRadians)
    {
        return 57.2958f * angleinRadians;
    }

    constexpr float toRadians(float angleInDegrees)
    {
        return 0.0174533f * angleInDegrees;
    }
}

Coords Transform::getWorldCoordinates() const 
{
    Coords o = this->parent;
    o = this->parent * this->local;
    float s = sinf(toRadians(o.rotation));
    float c = cosf(toRadians(o.rotation));

    wkt::math::mat2 m {
        { c * o.scaleX, s * o.scaleY },
        { -s * o.scaleX, c * o.scaleY }
    };

    o.localOrigin = this->local.localOrigin;
    o.rotationAnchor = this->local.rotationAnchor + o.localOrigin;
    o.position = this->parent.position + this->parent.localOrigin + m.transform(this->local.position);
    return o;
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