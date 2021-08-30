#pragma once

#include "W4Math.h"
#include "ArcBall.h"
#include "Nodes/Camera.h"

namespace w4::render {

class ArcBallNode: public core::DataNode
{
    W4_NODE(ArcBallNode, core::DataNode);
public:
    explicit ArcBallNode(const std::string& name = core::Object::defaultName, float radius = 1.f);
    ArcBallNode(NodeCloning, const ArcBallNode&);

    ~ArcBallNode();

    void initMath();
    void stop();

    void onPress(uint32_t x, uint32_t y);
    void onMove(uint32_t x, uint32_t y);
    void onRelease(uint32_t x, uint32_t y);
    virtual void onUpdate(float dt) override;

    void setFriction(float v);
    void setSensitivity(float v);
    void setRadius(float r);
    void setVelocity(const math::Rotator& v);

    float getRadius() const;

    void enableUpdate(bool flag = true);
    void disableUpdate();

    void enableInput(bool flag = true);
    void disableInput();

    void onTransformChange() override;
    void onResize(w4::event::Resize::cref evt);

private:
    void registerUpdate();
    void unregisterUpdate();

    void registerResize();
    void unregisterResize();

private:
    math::ArcBall m_arcball;
    float m_radius = 0.f;

    w4::sptr<w4::event::Resize::Handle> m_resizeHandle = nullptr;
    w4::sptr<w4::event::Touch::Begin::Handle> m_touchBegin = nullptr;
    w4::sptr<w4::event::Touch::Move::Handle> m_touchMove = nullptr;
    w4::sptr<w4::event::Touch::End::Handle> m_touchEnd = nullptr;
    w4::math::vec3 m_translation;

    std::optional<uint32_t> m_updateHandle;
};

}