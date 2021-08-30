#pragma once

#include "Nodes/DataNode.h"


namespace w4::render {

class PointLight: public core::DataNode
{
    W4_NODE(PointLight, core::DataNode);
public:
    PointLight(const std::string& name);
    PointLight(NodeCloning, const PointLight&);
    
    void setColor(const math::vec3& c);
    math::vec3 getColor() const;

    void setIntensity(float i);
    float getIntensity() const;

    void setDecayRate(core::LightDecayRate);
    core::LightDecayRate getDecayRate() const;
    float getAttenuation() const;

    bool isInFrustum(const render::Camera& cam) const override;

    void debugViewForeachSurface(std::function<void(core::Surface &)> visitor) override;

protected:
    void onDebugViewEnabled(bool) override;

protected:
    float m_intensity = 1.0f;
    math::vec3 m_color = {1.f, 1.f, 1.f};
    core::LightDecayRate m_decayRate = core::LightDecayRate::None;

    sptr<core::VisibleNode> m_debugView;
};

}
