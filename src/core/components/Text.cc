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
    this->opacity = t.opacity;
    this->ttf = t.ttf;

    if(!t.ss)
        this->ss = nullptr;
    else
    {
        this->ss = std::make_unique<wkt::gph::SmartSurface>(*t.ss);
        this->ss->setOpacity(this->opacity);
    }
}

Text& Text::operator=(const Text& t)
{
    this->text = t.text;
    this->color = t.color;
    this->opacity = t.opacity;
    this->ttf = t.ttf;

    if(!t.ss)
        this->ss = nullptr;
    else
    {
        this->ss = std::make_unique<wkt::gph::SmartSurface>(*t.ss);
        this->ss->setOpacity(this->opacity);
    }

    return *this;
}

void Text::setOpacity(uint8_t opacity)
{
    this->opacity = opacity;

    if(!this->ss)
        return;

    this->ss->setOpacity(opacity);
}

void Text::write(const std::string& text)
{
    this->text = text;
    s2x::Surface srf = this->ttf->write(text, this->color);
    this->ss = std::make_unique<wkt::gph::SmartSurface>(srf);
    this->ss->setOpacity(this->opacity);
}

void Text::draw(const wkt::gph::Director& director, const wkt::components::Transform& transform)
{
    director.shot(*this->ss, transform);
}

}}