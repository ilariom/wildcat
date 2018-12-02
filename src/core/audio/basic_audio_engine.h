#ifndef WKT_BASIC_AUDIO_ENGINE_H
#define WKT_BASIC_AUDIO_ENGINE_H

#include "audio_engine_types.h"
#include "AudioStudio.h"
#include "SoundEngineer.h"
#include <string>
#include <functional>

namespace wkt {
namespace audio {

class FelixTheCat : public SoundEngineer
{
public:
    FelixTheCat() : SoundEngineer("Felix The Cat")
    {
        insert("play-music", [] (Mixer& mixer, const SoundEvent& ev) {
            mixer.playMusic(ev.msg);
        });

        insert("play-fx", [this] (Mixer& mixer, const SoundEvent& ev) {
            mixer.bindChannel(ev.msg, this->channel);
            mixer.playChannel(this->channel);
            this->channel = (this->channel + 1) % AudioStudio::mixer_channels;
        });
    }

private:
    int channel = 0;
};

}}

#endif
