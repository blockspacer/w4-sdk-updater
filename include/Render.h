#pragma once

#include <vector>
#include <list>

#include "W4Common.h"
#include "RenderState.h"
#include "Node.h"
#include "Nodes/PointLight.h"
#include "RenderPass.h"
#include "Passes/NodesPass.h"
#include "Passes/FxPass.h"
#include "Passes/HudPass.h"

#include "Nodes/Camera.h"
#include "Timer.h"

namespace w4 {

struct RenderSettings
{
    uint8_t n_directionalLights = 1;
    uint8_t n_pointLights = 8;
    uint8_t n_spotLights = 4;
};

class Render
{
public:
    static void init(const RenderSettings& settings);

    static void resize(const math::size&);
    static void render();
    static void update(float);

    static float getElapsedTime();
    static const math::size& getSize();
    static const float getAspect();

    static size_t addPass();
    static const sptr<render::NodesPass>& getPass(size_t idx);

    static void enableFx(bool);
    static render::FxPass& getFxPass();

    static void enableHud(bool value);
    static render::HudPass& getHudPass();

    static void enableDebugView(bool value);
    static bool isDebugViewEnabled();

    static w4::sptr<render::Camera>        getScreenCamera();
    static void                    setScreenCamera(w4::cref<render::Camera>);

    static sptr<render::RootNode>  getRoot(size_t pass = 0);
    static void            setRoot(sptr<render::RootNode>, size_t pass = 0);

    static core::CollisionInfo raycast(const math::Ray& ray);
    static std::vector<core::CollisionInfo> raycastAll(const math::Ray& ray);

    static math::Ray createRayFromScreen(const math::point& p, size_t pass = 0);
    static math::Ray createRayFromScreenNormalized(const math::vec2& p, size_t pass = 0);
    static math::vec2 getScreenNormalizedFromWorldPoint(const math::vec3& p, size_t pass = 0);
    static math::vec2 getScreenNormalizedFromWorldPoint(const math::vec3& p, cref<render::Camera> camera);

    static void setOverrideMaterial(cref<resources::MaterialInst> mat);

    static bool hasDepthTextures();
    static bool hasFloatTextures();

    static const RenderSettings& getSettings();
private:
    Render(const RenderSettings& settings);

    void prepareGAPI();
    void cleanupGAPI();
    void internalRender();

private:
    float m_time = 0.0f;
    std::vector<sptr<render::NodesPass>>    m_nodesPassList;
    render::FxPass                          m_fxPass;
    render::HudPass                         m_hudPass;
    RenderSettings                          m_settings;

    w4::sptr<render::Camera> m_screenCamera;
    static Render* m_inst;

    static bool m_isDebugViewEnabled;
    static bool m_hasDepthTextures;
    static bool m_hasFloatTextures;
};

namespace render
{
    using Node = core::Node;
}

}// namespace


