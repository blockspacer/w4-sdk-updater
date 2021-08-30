
#pragma once

#include <memory>
#include <chrono>

#include "W4Common.h"
#include "TaskPool.h"

namespace w4::core {

class Timer
{
public:
    using TimerTaskPool = TaskPool<void (float)>;
    using Callable = std::function<bool (void)>;
    using hdl = TimerTaskPool::Handle;

    static void init();

    static hdl  addTask(float period, const Callable& func, bool isLooped = false);
    static bool removeTask(TimerTaskPool::Handle);
    static bool isActiveTask(TimerTaskPool::Handle);

    static float getDeltaTime();
    static time_t getCurrentSystemTime();

    static bool isEmpty();

    static void pause();
    static void resume();

    static void update();
    static void reset();
private:
    Timer();

    static Timer* m_instance;

    TimerTaskPool m_pool;
    time_t   m_snapshot;
    float    m_delta = 0.0f;
    bool     m_isPaused = false;
};

} // namespace

