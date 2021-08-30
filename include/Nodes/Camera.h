#pragma once

#include "W4Math.h"
#include "Nodes/DataNode.h"
#include "RenderTarget.h"
#include "Mesh.h"

namespace w4::render {

enum class ClearMask : int
{
    None       = 0,
    Color      = (1 << 0),
    Depth      = (1 << 1),
    Skybox     = (1 << 2),
    Background = (1 << 3),
};

ClearMask operator | (ClearMask a, ClearMask b);
ClearMask operator & (ClearMask a, ClearMask b);
ClearMask operator ^ (ClearMask a, ClearMask b);

class IBackgroundObject
{
public:
    virtual ~IBackgroundObject() {}

    void setMaterial(w4::cref<resources::MaterialInst>);
    w4::sptr<resources::MaterialInst> getMaterial();

    void onRender(const IRenderPass&);
protected:

    virtual w4::sptr<resources::MaterialInst> createDefaultMaterial() = 0;
    w4::sptr<Mesh> m_mesh;
};

class Background : public IBackgroundObject
{
public:
    Background();

    void setTexture(w4::cref<resources::Texture> texture);
protected:
    w4::sptr<resources::MaterialInst> createDefaultMaterial() override;
};

class Skybox : public IBackgroundObject
{
public:
    Skybox();

    void setLocalTransform(const math::Transform& localTransform);
    void setCubemap(w4::cref<resources::Cubemap> cubemap);
protected:
    w4::sptr<resources::MaterialInst> createDefaultMaterial() override;
};

class Camera
    : public core::DataNode
{
    W4_NODE(Camera, core::DataNode);
public:
    Camera(const std::string& name);
    Camera(const std::string& name, float fov, float aspect, float near, float far);
    Camera(const std::string& name, const math::size& screen, float near, float far);

    Camera(NodeCloning, const Camera&);

    void setupRenderState();

    void setOrtho(const math::size&, float near, float far);
    void setPerspective(float fov, float aspect, float near, float far);

    void setFov(float v);
    void setAspect(float v);
    void setNear(float v);
    void setFar(float v);

    float getFov() const;
    float getAspect() const;
    float getNear() const;
    float getFar() const;

    bool getIsOrtho() const;

    math::size getScreenSize() const;

    void setClearColor(const math::vec4& v);
    const math::vec4& getClearColor() const;
    void setClearMask(ClearMask v);
    ClearMask getClearMask() const;

    const math::mat4& getProjection() const;
    const math::mat4& getViewProjection() const;

    void setRenderTarget(w4::cref<IRenderTarget> target);
    w4::cref<IRenderTarget> getRenderTarget() const;

    bool hasSkybox() const;
    bool hasBackground() const;

    w4::cref<Skybox> getSkybox() const;
    w4::cref<Background> getBackground() const;
    const math::Frustum& getFrustum() const;

    math::Ray createRayFromScreen(const math::point& p) const;
    math::Ray createRayFromScreenNormalized(const math::vec2& p) const;
    math::vec2 getScreenNormalizedFromWorldPoint(const math::vec3& p) const;

private:
    void updateMatrix();
    void updateFrustumWorld();
    void updateFrustumLocal();
    void onTransformChange() override;

    float m_fov = 60.f;
    float m_aspect = 16.0f / 9.0f;

    float m_width  = 640.f;
    float m_height = 480.f;
    bool m_isOrtho = false;

    float m_near = 0.1f;
    float m_far = 1000.0f;

    ClearMask m_clearMask = ClearMask::Color | ClearMask::Depth;
    math::vec4 m_clearColor = {0.06f, 0.13f, 0.2f, 1.0f};

    math::mat4 m_projection = math::mat4::identity;
    math::mat4 m_view = math::mat4::identity;
    math::mat4 m_projectionView = math::mat4::identity;

    w4::sptr<IRenderTarget> m_renderTarget = ScreenRenderTarget::get();

    w4::sptr<Background> m_background = w4::make::sptr<Background>();
    w4::sptr<Skybox>     m_skybox = w4::make::sptr<Skybox>();

    math::Frustum m_frustum;
};

}