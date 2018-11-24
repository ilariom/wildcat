#include "Text.h"
#include "graphics/FontCache.h"

namespace wkt {
namespace components
{

Text::Text(const std::string& fontPath, int fontSize)
{
    auto& fontCache = wkt::gph::FontCache::getInstance();
    this->ttf = fontCache(fontPath, fontSize);
}

Text::Text(const Text& t)
{
    this->text = t.text;
    this->color = t.color;
    this->ttf = t.ttf;

    if(!t.ss)
        this->ss = nullptr;
    else
        this->ss = std::make_unique<wkt::gph::SmartSurface>(*t.ss);
}

Text& Text::operator=(const Text& t)
{
    this->text = t.text;
    this->color = t.color;
    this->ttf = t.ttf;

    if(!t.ss)
        this->ss = nullptr;
    else
        this->ss = std::make_unique<wkt::gph::SmartSurface>(*t.ss);

    return *this;
}

void Text::write(const std::string& text)
{
    this->text = text;
    s2x::Surface srf = this->ttf->write(text, this->color);
    this->ss = std::make_unique<wkt::gph::SmartSurface>(srf);
}

void Text::draw(const wkt::gph::Director& director, const wkt::components::Transform& transform)
{
    director.shot(*this->ss, transform);
}

}}