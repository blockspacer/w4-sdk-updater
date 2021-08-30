#pragma once

#include "Nodes/DataNode.h"

namespace w4::render {

class SpotLight: public core::DataNode
{
    W4_NODE(SpotLight, core::DataNode);
public:
    SpotLight(const std::string& name);
    SpotLight(NodeCloning, const SpotLight&);

    void setAngle(float r);
    float getAngle() const;

    void setColor(const w4::math::vec3& c);
    w4::math::vec3 getColor() const;

    void setIntensity(float i);
    float getIntensity() const;


    void setDecayRate(core::LightDecayRate);
    core::LightDecayRate getDecayRate() const;

    void setDecayFactor(float f);
    float getDecayFactor() const;

    void enableShadows(bool flag = true);
    bool isCastingShadows() const;

    void debugViewForeachSurface(std::function<void(core::Surface &)> visitor) override;

protected:
    void onDebugViewEnabled(bool) override;

protected:
    float m_angle = math::HALF_PI / 2.0f;
    float m_decayFactor = 0.0f;
    float m_intensity = 1.0f;
    math::vec3 m_color = {1.f, 1.f, 1.f};

    bool m_castShadows = false;

    core::LightDecayRate m_decayRate = core::LightDecayRate::None;
    sptr<core::Node> m_debugView;
};

}
