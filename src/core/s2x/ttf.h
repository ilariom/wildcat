#ifndef S2X_TTF_H
#define S2X_TTF_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "s2x/video.h"
#include <string>

namespace s2x
{

class TrueTypeFont final
{
public:
    inline TrueTypeFont(const std::string& fontPath, int fontSize);
    TrueTypeFont(const TrueTypeFont&) = delete;
    TrueTypeFont(TrueTypeFont&&) = delete;
    ~TrueTypeFont() { TTF_CloseFont(this->resource); }

    TrueTypeFont& operator=(const TrueTypeFont&) = delete;
    TrueTypeFont& operator=(TrueTypeFont&&) = delete;

public:
    operator TTF_Font*() const { return this->resource; }
    const std::string& getFontPath() const { return this->fontPath; }
    int getFontSize() const { return this->fontSize; }

    Surface write(const std::string& text, SDL_Color color) { return Surface(TTF_RenderText_Blended(*this, text.c_str(), color)); }

private:
    std::string fontPath;
    int fontSize;
    TTF_Font* resource = nullptr;
};

inline TrueTypeFont::TrueTypeFont(const std::string& fontPath, int fontSize) : fontPath(fontPath), fontSize(fontSize) 
{ 
    this->resource = TTF_OpenFont(fontPath.c_str(), fontSize);
}

}

#endif