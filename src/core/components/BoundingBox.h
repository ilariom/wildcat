#ifndef WKT_BOUNDING_BOX
#define WKT_BOUNDING_BOX

#include "ecs/Component.h"
#include "math/wktmath.h"
#include <memory>

namespace wkt {
namespace components
{

struct BoundingBox : public wkt::ecs::Component, public wkt::math::Rect
{ 
    virtual void update() { };
};

template<typename sz_src, typename pos_src, template<typename> class S = std::shared_ptr>
class BB : public BoundingBox
{
public:
    void bindSizeTo(S<sz_src>& comp) { this->szcomp = comp; }
    void bindPositionTo(S<pos_src>& comp) { this->poscomp = comp; }
    inline void update() override;

private:
    S<pos_src> poscomp;
    S<sz_src> szcomp;
};

template<typename sz_src, typename pos_src, template<typename> class S>
inline void BB<sz_src, pos_src, S>::update()
{
    if(this->poscomp)
        this->origin = this->poscomp->getPosition();

    if(this->szcomp)
        this->size = this->szcomp->size();
}

struct FakePosSrc { wkt::math::vec2 zero; wkt::math::vec2& getPosition() { return zero; } };

template<typename sz_src, typename pos_src = FakePosSrc>
std::shared_ptr<BoundingBox> makeBoundingBox(std::shared_ptr<sz_src> szcomp = nullptr, std::shared_ptr<pos_src> poscomp = nullptr)
{
    auto bb = std::make_shared<BB<sz_src, pos_src, std::shared_ptr>>();

    if(szcomp)
        bb->bindSizeTo(szcomp);

    if(poscomp)
        bb->bindPositionTo(poscomp);

    if(szcomp || poscomp)
        bb->update();

    return std::static_pointer_cast<BoundingBox>(bb);
}

template<typename sz_src, typename pos_src = FakePosSrc>
std::shared_ptr<BB<sz_src, pos_src>> getBoundingBox(std::shared_ptr<BoundingBox> bb)
{
    return std::static_pointer_cast<BB<sz_src, pos_src>>(bb);
}

REGISTER_COMPONENT(BoundingBox, -14);

}}

#endif