#pragma once

#include "IOuterID.h"
#include "Object.h"

namespace w4::platform {

class Audio final : public core::IOuterManaged
                  , public core::Object
{
    W4_OBJECT(Audio, core::Object);
    Audio(const std::string & source);

public:
    static void initialize();
    static void playStandalone(const std::string & source, float gain, bool isLooped);

    static void setGlobalGain(float gain);
    static float getGlobalGain();
    void outerCreator() override;
    void outerDeleter() override;

    void play();
    void pause();
    void resume();
    void stop();

    void setGain(float);
    void setGainLinear(float, float timeOffset);

    float getGain() const;

    void setLooped(bool);
    bool isLooped() const;

    bool isPlaying() const;

private:
    std::string const m_source;
    bool m_looped = false;
};

} //w4::platform
