#ifndef WKT_ATLAS_H
#define WKT_ATLAS_H

#include "graphics/Flipbook.h"

namespace wkt {
namespace gph
{

class Atlas : public Flipbook
{
public:
    using atlas_iterator = flipbook_iterator;
};

}}

#endif