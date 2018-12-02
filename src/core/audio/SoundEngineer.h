#ifndef WKT_SOUND_ENGINEER_H
#define WKT_SOUND_ENGINEER_H

#include "audio_engine_types.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace wkt {
namespace audio
{

class SoundEngineer
{
public:
    explicit SoundEngineer(const std::string& name) : name(name) { }
    SoundEngineer() = default;

public:
    void insert(const std::string& eventName, const SoundEventResponder& responder) { this->responses[eventName] = responder; }
    void erase(const std::string& eventName) { this->responses.erase(eventName); }
    inline void answer(Mixer& mixer, const SoundEvent& ev);
    const std::string& getName() const { return this->name; }

private:
    std::string name;
    std::unordered_map<std::string, SoundEventResponder> responses;
};

inline void SoundEngineer::answer(Mixer& mixer, const SoundEvent& ev)
{
    if(this->responses.find(ev.name) == this->responses.end())
        return;

    this->responses[ev.name](mixer, ev);
}

}}

#endif