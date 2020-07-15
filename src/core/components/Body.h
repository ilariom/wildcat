#ifndef WKT_BODY_H
#define WKT_BODY_H

#include "ecs/Component.h"
#include "math/wktmath.h"
#include <vector>

namespace wkt {
namespace components
{

class Transform;

class Body : public wkt::ecs::Component
{
public:
    struct Box
    {
        float x0, y0, x1, y1;

        wkt::math::vec2 topLeft() const;
        wkt::math::vec2 topRight() const;
        wkt::math::vec2 bottomLeft() const;
        wkt::math::vec2 bottomRight() const;

        float width() const;
        float height() const;

        wkt::math::Size size() const;

        bool contains(const wkt::math::vec2&, const wkt::math::vec2&) const;
    };

    struct Config
    {
        float mass;
        float torque;

        Config()
            : mass(1)
            , torque(0)
        { }
    };

public:
    Body(
        wkt::components::Transform& transform,
        const Config& conf = {}
    ) 
        : transform(transform)
        , conf(conf)
    { }

public:
    bool hit(const wkt::math::vec2&) const;
    bool hit(const Box&) const;

    void push(const wkt::math::vec2&) const;
    void torque(float) const;

    std::vector<Box>& getBoxes() { return this->boxes; }
    const std::vector<Box>& getBoxes() const { return this->boxes; }

    bool unique() const override;

private:
    std::vector<Box> boxes;
    Config conf;
    wkt::components::Transform& transform;
};

REGISTER_COMPONENT(Body, -17);

}}

#endif