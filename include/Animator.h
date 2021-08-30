#pragma once

#include <unordered_map>
#include <map>
#include <functional>

#include "Object.h"

namespace w4::render {

class Animator
    : public core::Object
{
    W4_OBJECT(Animator, core::Object);

public:
    enum class PlayType
    {
        Forward,
        Backward
    };

    enum class State
    {
        Idle,
        Play,
        Pause
    };

    using StateHandler = std::function<void(Animator&)>;
    using TimeHandler = std::function<void(Animator&)>;
    using StateHandlerId = uint32_t;

public:
    explicit Animator(const std::string& name);
    virtual ~Animator() noexcept;

    void play();
    void pause();
    void resume();
    void stop();

    void seek(float targetTime);
    virtual void onUpdate(float dt) override;

    bool isLooped() const;
    float getSpeed() const;
    State getState() const;
    PlayType getPlayType() const;
    float getCurrentTime() const;

    void setIsLooped(bool);
    void setSpeed(float);
    void setPlayType(PlayType);
    void setBeginTime(float);
    void setEndTime(float);

    StateHandlerId setStateHandler(State state, StateHandler handler);
    void resetStateHandler(StateHandlerId handler);
    void setTimeHandler(float dt, TimeHandler handler);

    virtual float getFps() const = 0;
    virtual float getDuration() const = 0;

protected:
    virtual void processUpdate(float currentTime) = 0;

private:
    void updateState(State);

private:
    State m_state = State::Idle;
    PlayType m_playType = PlayType::Forward;
    float m_speed = 1.f;
    bool m_isLooped = false;

    float m_timePassed = 0.f;
    float m_timeBegin = 0.f;
    float m_timeEnd = 0.f;

    StateHandlerId m_stateHandlerId = 0;
    std::unordered_map<State, std::unordered_map<uint32_t, StateHandler>> m_stateHandlers;
    std::map<float, TimeHandler> m_timeHandlers;
};

} //namespace w4::render
