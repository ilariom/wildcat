#ifndef S2X_MIXER_H
#define S2X_MIXER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <cassert>

namespace s2x
{

class Mixer final
{
public:
    constexpr static int frequency = 22050;
    constexpr static int chunksize = 2048;

    constexpr static int PLAY_INFINITE = -1;

public:
    inline Mixer(int channels);
    Mixer(const Mixer&) = delete;
    Mixer(Mixer&&) = delete;
    inline ~Mixer();

    Mixer& operator=(const Mixer&) = delete;
    Mixer& operator=(Mixer&&) = delete;

public:
    inline bool loadEffect(const std::string& filePath);
    inline bool loadMusic(const std::string& filePath);
    inline void releaseEffect(const std::string& filePath);
    inline void releaseMusic(const std::string& filePath);

    // CHANNELS

    inline int bindChannel(const std::string& filePath, int channel);

    inline void playChannel(int channel, int loops = 1, int fadeIn = 0, int timeout = -1);
    inline void haltChannel(int channel, int fadeOut = 0);
    
    void volume(int channel, float vol) { Mix_Volume(channel, (int)(vol * MIX_MAX_VOLUME)); }
    void mute(int channel) { Mix_Pause(channel); }
    void unmute(int channel) { Mix_Resume(channel); }
    void pan(int channel, float leftVol, float rightVol) { Mix_SetPanning(channel, (Uint8)(leftVol * 255), (Uint8)(rightVol * 255)); }
    void distance(int channel, uint8_t distance) { Mix_SetDistance(channel, (Uint8)distance); }
    inline void spatial(int channel, int16_t angle, uint8_t distance);
    
    bool isPlaying(int channel) const { return (bool) Mix_Playing(channel); }
    bool isPaused(int channel) const { return (bool) Mix_Paused(channel); }

    int getChannelsCount() const { return this->channelsCount; }

    // BGM

    inline void playMusic(const std::string& filePath, int loops = -1, int fadeIn = 0);
    inline void haltMusic(int fadeOut = 0);
    void setMusicPosition(double seconds) { Mix_SetMusicPosition(seconds); }
    void rewindMusic() { Mix_RewindMusic(); }
    void pauseMusic() { Mix_PauseMusic(); }
    void resumeMusic() { Mix_ResumeMusic(); }
    void musicVolume(float vol) { Mix_VolumeMusic((int)(vol * MIX_MAX_VOLUME)); }
    bool isMusicPaused() const { return (bool) Mix_PausedMusic(); }
    bool isMusicPlaying() const { return (bool) Mix_PlayingMusic(); }

private:
    std::unordered_map<std::string, std::pair<Mix_Chunk*, int>> fxs;
    std::unordered_map<std::string, std::pair<Mix_Music*, int>> bgms;
    std::unordered_map<int, Mix_Chunk*> boundChannels;
    int channelsCount;
    Mix_Music* currentBGM = nullptr;
};

inline Mixer::Mixer(int channels)
{
    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC);
    Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, 2, chunksize);
    Mix_AllocateChannels(channels);
    this->channelsCount = channels;
}

inline Mixer::~Mixer()
{
    Mix_CloseAudio();
    Mix_Quit();
}

inline bool Mixer::loadEffect(const std::string& filePath)
{
    auto it = this->fxs.find(filePath);

    if(it != this->fxs.end())
    {
        this->fxs[filePath].second++;
        return true;
    }

    std::pair<Mix_Chunk*, int> p = std::make_pair(
        Mix_LoadWAV(filePath.c_str()),
        1
    );

    if(p.first == nullptr)
        return false;

    this->fxs[filePath] = std::move(p);
    return true;
}

inline bool Mixer::loadMusic(const std::string& filePath)
{
    auto it = this->bgms.find(filePath);

    if(it != this->bgms.end())
    {
        this->bgms[filePath].second++;
        return true;
    }

    std::pair<Mix_Music*, int> p = std::make_pair(
        Mix_LoadMUS(filePath.c_str()),
        1
    );

    if(p.first == nullptr)
        return false;

    this->bgms[filePath] = std::move(p);
    return true;
}

inline void Mixer::releaseEffect(const std::string& filePath)
{
    if(this->fxs.find(filePath) == this->fxs.end())
        return;

    auto p = this->fxs[filePath];
    p.second--;

    if(p.second <= 0)
    {
        Mix_FreeChunk(p.first);
        this->fxs.erase(filePath);
    }
}

inline void Mixer::releaseMusic(const std::string& filePath)
{
    if(this->bgms.find(filePath) == this->bgms.end())
        return;

    auto p = this->bgms[filePath];
    p.second--;

    if(p.second <= 0)
    {
        Mix_FreeMusic(p.first);
        this->bgms.erase(filePath);
    }
}

inline int Mixer::bindChannel(const std::string& filePath, int channel)
{
    if(this->fxs.find(filePath) == this->fxs.end())
    {
        bool res = loadEffect(filePath);
        assert(res);
    }

    this->boundChannels[channel] = this->fxs[filePath].first;
    return channel;
}

inline void Mixer::playChannel(int channel, int loops, int fadeIn, int timeout)
{
    if(this->boundChannels.find(channel) == this->boundChannels.end())
    {
        return;
    }

    Mix_Chunk* p = this->boundChannels[channel];

    if(fadeIn > 0)
        Mix_FadeInChannelTimed(channel, p, loops, fadeIn, timeout);
    else
        Mix_PlayChannel(channel, p, loops);
}

inline void Mixer::haltChannel(int channel, int fadeOut)
{
    if(fadeOut > 0)
        Mix_FadeOutChannel(channel, fadeOut);
    else
        Mix_HaltChannel(channel);
}

inline void Mixer::spatial(int channel, int16_t angle, uint8_t distance)
{
    if(angle == 0 && distance == 0)
        distance = 1;

    Mix_SetPosition(channel, (Sint16)angle, (Uint8)distance);
}

inline void Mixer::playMusic(const std::string& filePath, int loops, int fadeIn)
{
    if(this->bgms.find(filePath) == this->bgms.end())
    {
        bool res = loadMusic(filePath);
        assert(res);
    }

    Mix_Music* m = this->bgms[filePath].first;

    if(fadeIn > 0)
        Mix_FadeInMusic(m, loops, fadeIn);
    else
        Mix_PlayMusic(m, loops);

    this->currentBGM = m;
}

inline void Mixer::haltMusic(int fadeOut)
{
    if(!this->currentBGM)
        return;

    if(fadeOut > 0)
        Mix_FadeOutMusic(fadeOut);
    else
        Mix_HaltMusic();

    this->currentBGM = nullptr;
}

}

#endif