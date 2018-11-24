#ifndef WKT_TEXT_H
#define WKT_TEXT_H

#include "ecs/Drawable.h"
#include "graphics/SmartSurface.h"
#include "graphics/Color.h"
#include "s2x/ttf.h"
#include <string>
#include <memory>

namespace wkt {
namespace components
{

class Text : public wkt::ecs::Drawable
{
public:
    Text(const std::string& fontPath, int fontSize);
    Text(const Text&);
    Text(Text&&) = default;
    ~Text() = default;

    Text& operator=(const Text&);
    Text& operator=(Text&&) = default;

public:
    void setColor(const wkt::gph::Color& color) { this->color = color; }
    void write(const std::string& text);
    const wkt::gph::Color& getColor() const { return this->color; }
    const std::string& getWrittenText() const { return this->text; }

    void draw(const wkt::gph::Director&, const wkt::components::Transform&) override;

private:
    std::string text;
    wkt::gph::Color color = wkt::gph::colors::WHITE;
    std::shared_ptr<s2x::TrueTypeFont> ttf;
    std::unique_ptr<wkt::gph::SmartSurface> ss = nullptr;
};

REGISTER_DRAWABLE(Text, -13);

}}

#endif