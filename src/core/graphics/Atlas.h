#ifndef WKT_ATLAS_H
#define WKT_ATLAS_H

#include "graphics/Flipbook.h"
#include <string>

namespace wkt {
namespace gph
{

class Atlas : public Flipbook
{
public:
    using atlas_iterator = flipbook_iterator;

public:
    Atlas() = default;
    
    Atlas(const std::string& name)
        : name(name)
    { }

public:
    void setName(const std::string& name) { this->name = name; }
    const std::string& getName() const { return this->name; }

private:
    std::string name;
};

}}

#endif