#ifndef WKT_AUDIO_STUDIO_H
#define WKT_AUDIO_STUDIO_H

#include "audio_engine_types.h"
#include "SoundEngineer.h"
#include <unordered_map>
#include <string>
#include <cassert>

namespace wkt {
namespace audio
{

class AudioStudio final
{
public:
    constexpr static int mixer_channels = 32;

public:
    static inline AudioStudio& getInstance();

    AudioStudio(const AudioStudio&) = delete;
    AudioStudio(AudioStudio&&) = delete;

    AudioStudio& operator=(const AudioStudio&) = delete;
    AudioStudio& operator=(AudioStudio&&) = delete;

public:
    inline void ask(const std::string& engineerName);
    inline void tell(const SoundEvent& event);
    inline void tell(const std::string& engineerName, const SoundEvent& event);

    void hire(const SoundEngineer& se) { this->engineers[se.getName()] = se; }
    void hire(SoundEngineer&& se) { this->engineers[se.getName()] = std::move(se); }

private:
    AudioStudio() : mixer(mixer_channels) { }
    ~AudioStudio() = default;

private:
    Mixer mixer;
    std::unordered_map<std::string, SoundEngineer> engineers;
    SoundEngineer* currentEngineer = nullptr;
};

inline void AudioStudio::ask(const std::string& engineerName)
{
    assert(this->engineers.find(engineerName) != this->engineers.end());
    this->currentEngineer = &this->engineers[engineerName];
}

inline void AudioStudio::tell(const SoundEvent& event)
{
    if(!this->currentEngineer)
        return;

    this->currentEngineer->answer(this->mixer, event);
}

inline void AudioStudio::tell(const std::string& engineerName, const SoundEvent& event)
{
    auto oldEng = this->currentEngineer;
    ask(engineerName);
    tell(event);
    this->currentEngineer = oldEng;
}

inline AudioStudio& AudioStudio::getInstance()
{
    static AudioStudio as;
    return as;
}

}}

#endif