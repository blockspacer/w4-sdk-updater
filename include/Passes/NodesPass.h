#pragma once
#include "RenderPass.h"

#include "Nodes/PointLight.h"
#include "Nodes/SpotLight.h"

namespace w4::render {

class DirectionalLight
{
public:

    DirectionalLight(const math::vec3& color = math::vec3(0.0, 0.0, 0.0), const math::vec3& direction = math::vec3(0.0, 0.0, 1.0));

    void setColor(const math::vec3& color);
    void setDirection(const math::vec3& direction);

    const math::vec3& getColor() const;
    const math::vec3& getDirection() const;

    void enableShadows(bool flag = true);
    bool isCastingShadows() const;
private:
    math::vec3 m_color;
    math::vec3 m_direction;

    bool m_castShadows = false;
};

class NodesPass : public IRenderPass
{
public:
    NodesPass();

    void begin() override {}
    void draw() override;
    void end() override {}

    void setDirty();

    cref<Camera> getCamera() const override;
    void setCamera(cref<Camera>);

    void setRoot(cref<RootNode>);
    cref<RootNode> getRoot() const;

    void setPreRenderHandler(const std::function<void ()>&);
    void setPostRenderHandler(const std::function<void ()>&);
    void setSpotLightShadowMapSize(uint32_t size);
    void setDirectionalLightShadowMapSize(uint32_t size);

    sptr<DirectionalLight> getDirectionalLight();

    void setDirectionalLightShadowMaxDistance(float value);

private:
    void renderSurface(render::Surface* surface);
    void rebuild(cref<core::Node> root);

    void renderMain();
    void renderDirectionalLightShadowMap();
    void renderSpotLightShadowMap();

    void initDirectionalShadowMap(uint32_t size);
    void initSpotShadowMap(uint32_t size);

private:
    bool m_isDirty = true;
    std::multimap<int, render::Surface*> m_blending;
    std::unordered_map<resources::Material*,
            std::unordered_map<resources::MaterialInst*,
            std::unordered_map<resources::IVerticesBuffer*, std::vector<render::Surface*>
    >
    >
    > m_opaque;

    sptr<RootNode>  m_root;
    uint32_t m_handlerId;

    sptr<Camera>  m_camera;
    sptr<DirectionalLight>          m_directionalLight;
    std::vector<sptr<PointLight>>   m_pointLights;
    std::vector<sptr<SpotLight>>    m_spotLights;

    sptr<TextureRenderTarget>       m_directionalShadowMapTarget;
    sptr<resources::MaterialInst>   m_directionalShadowMapMaterial;
    uint32_t                        m_directionalShadowMapSize = 1024;
    bool m_needDirectionalLightShadowMap = false;

    sptr<TextureRenderTarget>       m_spotShadowMapTarget;
    sptr<resources::MaterialInst>   m_spotShadowMapMaterial;
    std::array<math::vec2, 4>       m_spotShadowMapRTBeginCoords;
    uint32_t                        m_spotShadowMapSize = 1024;
    bool m_needSpotLightShadowMap = false;

    std::function<void ()> m_preRenderHandler;
    std::function<void ()> m_postRenderHandler;

    float m_directionalShadowMaxDistance = -1;
};

}