#ifndef WKT_AUDIO_ENGINE_TYPES_H
#define WKT_AUDIO_ENGINE_TYPES_H

#include "s2x/mixer.h"
#include <string>
#include <functional>

namespace wkt {
namespace audio
{

using Mixer = s2x::Mixer;
class AudioStudio;
class SoundEngineer;

struct SoundEvent
{
    std::string name;
    std::string msg;
};

inline bool operator==(const SoundEvent& a, const SoundEvent& b)
{
    return a.name == b.name;
}

struct SoundEventHasher
{
    size_t operator()(const SoundEvent& se) const
    {
        return std::hash<std::string>()(se.name);
    }
};

using SoundEventResponder = std::function<void(Mixer&, const SoundEvent&)>;

}}

#endif