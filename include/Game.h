#pragma once

#include <memory>
#include <deque>

#include "Utils.h"
#include "Render.h"
#include "TaskPool.h"
#include "Timer.h"
#include "Platform.h"
#include "Input.h"
#include "VFS.h"
#include "Nodes/Root.h"


namespace w4 {

struct FSMState
{
    virtual void onEnter()        {}
    virtual void onLeave()        {}
    virtual void onUpdate(float)  {}
};


struct IGame
{
    virtual ~IGame() {}

    virtual void onConfig()           {}
    virtual void onStart()            {}
    virtual void onUpdate(float dt)   {}
    virtual void onExit()             {}
    virtual void onPause()            {}
    virtual void onResume()           {}
    virtual void onLost()             {}
    virtual void onRestore()          {}

    virtual void onTouch(const event::Touch::Begin& evt) {}
    virtual void onKey(const event::Keyboard::Down& evt) {}
    virtual void onResize(const math::size&) {}

};

class Game
{
public:
    using UpdatePool = core::TaskPool<void (float)>;
    struct cfg
    {
        bool VFSInit   = true;
        bool VFSClean  = true;
        bool UseSimpleInput = true;
        bool EnableFrustumCulling = false;
        bool UseDefaultRenderPass = true;
        bool StopUpdateWhenFocusLoss = true;
        RenderSettings RSettings;

        const char * VFSConfig = "config_fs.json";
        const char * RemoteServerAddress = "http://localhost:9696";
        void log();
    };

public:

    static cfg Config;
    static w4::sptr<Game> getInstance();

    ~Game() = default;

    void onConfig();
    void onStart();
    void onExit();
    void onPause();
    void onResume();
    bool onLost();
    bool onRestore();
    void onUpdate(float dt);

    long getCurrentFrame() const;


    bool draw();
    template<typename Impl> static void run(const char* parentObject);

    UpdatePool& getUpdatePool();

    void setTimeScale(float s);
    float getTimeScale() const;
private:

    Game(const w4::sptr<IGame>& impl, const char* parentObject);

    void initUpdate();
    void startUpdate();
    void stopUpdate();

    static w4::sptr<Game> m_instance;

    w4::sptr<IGame> m_impl;
    UpdatePool      m_updatePool;

    std::function<void (float)> m_update;

    bool m_gameLoopStarted = false;
    bool m_onFocus = false;

    float m_timeScale = 1.f;

    long m_currentFrame = 0;
};

#include "impl/Game.inl"

}
