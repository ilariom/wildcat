#ifndef _SKR_TRANSFORM_H
#define _SKR_TRANSFORM_H

#include "ecs/Component.h"
#include "math/wktmath.h"
#include <cassert>
#include <math.h>
#include <limits>

namespace wkt {
namespace components
{

struct Coords
{
    wkt::math::vec2 position { 0, 0 };
    wkt::math::vec2 rotationAnchor { 0, 0 };
    float rotation = 0;
    float scaleX = 1;
    float scaleY = 1;

    Coords& operator*=(const Coords& other);
};

Coords operator*(const Coords& a, const Coords& b);

class Transform : public wkt::ecs::Component
{
private:
    using ComponentTypeID = wkt::ecs::Component::ComponentTypeID;

public:
    bool unique() const override { return true; }

    inline void setPosition(float x, float y);
    inline void setPosition(const wkt::math::vec2& position);
    inline void setRotation(float rotation);
    inline void setRotationAnchor(const wkt::math::vec2& ra);
    inline void setScale(float scale);
    inline void setScaleX(float scaleX);
    inline void setScaleY(float scaleY);
    inline void setScale(float scaleX, float scaleY);

    const wkt::math::vec2& getPosition() const { return this->local.position; }
    float getRotation() const { return this->local.rotation; }
    const wkt::math::vec2& getRotationAnchor() const { return this->local.rotationAnchor; }
    float getScaleX() const { return this->local.scaleX; }
    float getScaleY() const { return this->local.scaleY; }
    float getScale() const { assert(getScaleX() == getScaleY()); return getScaleX(); }

    void addPosition(const wkt::math::vec2& moveBy) { setPosition(getPosition() + moveBy); }
    void addRotation(float rotateBy) { setRotation(getRotation() + rotateBy); }
    void addScaleX(float x) { setScaleX(getScaleX() + x); }
    void addScaleY(float y) { setScaleY(getScaleY() + y); }
    void addScale(float s) { setScale(getScale() + s); }

    inline Transform& operator*=(const Transform& other);
    inline Transform& invert();

    inline wkt::math::vec2 operator*(const wkt::math::vec2& v);

    void setParentCoordinates(Coords coords) { this->parent = std::move(coords); }
    const Coords& getParentCoordinates() const { return this->parent; }
    Coords getWorldCoordinates() const;
    const Coords& getCoordinates() const { return this->local; }

private:
    Coords local;
    Coords parent;
};

inline void Transform::setPosition(float x, float y)
{
    this->local.position.x = x;
    this->local.position.y = y;
}

inline void Transform::setPosition(const wkt::math::vec2 &position)
{
    this->local.position = position;
}

inline void Transform::setRotation(float rotation)
{
    this->local.rotation = rotation;
}

inline void Transform::setRotationAnchor(const wkt::math::vec2& ra)
{
    this->local.rotationAnchor = ra;
}

inline void Transform::setScale(float scale)
{
    setScale(scale, scale);
}

inline void Transform::setScaleX(float scaleX)
{
    this->local.scaleX = std::max(std::numeric_limits<float>::epsilon(), scaleX);
}

inline void Transform::setScaleY(float scaleY)
{
    this->local.scaleY = std::max(std::numeric_limits<float>::epsilon(), scaleY);
}

inline void Transform::setScale(float scaleX, float scaleY)
{
    this->local.scaleX = std::max(std::numeric_limits<float>::epsilon(), scaleX);
    this->local.scaleY = std::max(std::numeric_limits<float>::epsilon(), scaleY);
}

inline Transform& Transform::operator*=(const Transform& other)
{
    this->local *= other.local;

    return *this;
}

inline Transform& Transform::invert()
{
    this->local.position *= -1;
    this->local.scaleX = 1.f / this->local.scaleX;
    this->local.scaleY = 1.f / this->local.scaleY;
    this->local.rotation *= -1;

    return *this;
}

inline wkt::math::vec2 Transform::operator*(const wkt::math::vec2& v)
{
    auto s = sinf(this->local.rotation);
    auto c = cosf(this->local.rotation);

    wkt::math::vec2 row1 { -s * this->local.scaleX, c };
    wkt::math::vec2 row2 { c, s * this->local.scaleY };
    
    return wkt::math::vec2 {
        this->local.position.x + row1 * v,
        this->local.position.y + row2 * v
    };
}

REGISTER_COMPONENT(Transform, -2);

}}

#endif