#pragma once
#include <optional>

#include "IOuterID.h"
#include "Object.h"
#include "Texture.h"

namespace w4::platform {

class Video final : public core::IOuterManaged
                 , public core::Object
{
    W4_OBJECT(Video, core::Object);

public:
    using Handler = std::function<void()>;
    Video(const std::string& url);
    Video(const std::string& url, const Handler& onCanPlayHandler);
    ~Video();

    void play(bool looped = false);
    void pause();
    void stop();

    void mute();
    void unmute();

    void setMute(bool muted);
    void setLoop(bool looped);

    bool isMuted() const;
    bool isLooped() const;

    bool isPlaying() const;

    float getDuration() const;

    void setCurrentTime(float);
    float getCurrentTime() const;

    void setPlaybackRate(float);
    float getPlaybackRate() const;

    void setVolume(float);
    float getVolume() const;

    uint getWidth() const;
    uint getHeight() const;

    void outerCreator() override final;
    void outerDeleter() override final;

    void onEnded(const Handler& handler);

    w4::sptr<resources::VideoTexture> getTexture() const;

private:
    void onUpdate();

    std::optional<uint32_t> m_updateHandle;
    void registerUpdate();
    void unregisterUpdate();

    Handler m_canPlayHandler = nullptr;
    Handler m_endedHandler   = nullptr;

    static void doEnded(w4::core::OuterID::ndxType id);
    static void doCanPlay(w4::core::OuterID::ndxType id);

    static std::unordered_map<w4::core::OuterID::ndxType, Video*> m_videos;

    w4::sptr<resources::VideoTexture> m_texture;
    std::string m_url;
};

}