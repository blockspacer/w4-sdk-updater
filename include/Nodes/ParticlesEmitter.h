#pragma once

#include "ParticlesEmitterParameters.h"
#include "Nodes/VisibleNode.h"
#include "PodGenerator.h"
#include "Resource.h"

POD_STRUCT(ParticlesEmitterVertexFormat,
           POD_FIELD(w4::math::vec3, w4_a_position)
           POD_FIELD(w4::math::vec4, w4_a_transform0)
           POD_FIELD(w4::math::vec4, w4_a_transform1)
           POD_FIELD(w4::math::vec4, w4_a_transform2)
           POD_FIELD(w4::math::vec4, w4_a_transform3)
           POD_FIELD(w4::math::vec4, w4_a_color)
           POD_FIELD(w4::math::vec2, w4_a_uv)
);

namespace w4::render {

class ParticlesEmitter final
    : public core::VisibleNode
    , public core::Cache<ParticlesEmitter, sptr<ParticlesEmitter>, resources::ResourceFileDescription, core::CloneCreator<ParticlesEmitter, sptr<ParticlesEmitter>>>
{
    W4_NODE(ParticlesEmitter, core::VisibleNode);

    explicit ParticlesEmitter(const std::string &name);
public:
    enum class State
    {
        IDLE,
        STARTED,
        PAUSED
    };
    using StateChangedCallback = std::function<void(const ParticlesEmitter &, State)>;
    using StateChangedCallbackHandle = std::size_t;
    using VertexFormat = ParticlesEmitterVertexFormat;
public:
    static w4::sptr<ParticlesEmitter> load(const resources::ResourceFileDescription & source);

public:
    ParticlesEmitter(NodeCloning, const ParticlesEmitter& from);
    ~ParticlesEmitter();

    void start();
    void pause();
    void resume();
    void stop();

    bool hasActiveParticles() const;

    void setDepthStep(float);
    float getDepthStep() const;

    State getState() const;

    StateChangedCallbackHandle addStateChangedCallback(StateChangedCallback);
    void removeStateChangedCallback(StateChangedCallbackHandle);

    const ParticlesEmitterParameters & getParameters() const;
    void setParameters(const ParticlesEmitterParameters&);

    void setMaxParticles(uint32_t);
    void setAngle(float rads);
    void setAngleVariance(float rads);
    void setDuration(float seconds);
    void setBlendFactorSrc(BlendFactor);
    void setBlendFactorDst(BlendFactor);
    void setBlendFunc(BlendFunc);
    void setBlendFunc(BlendFactor src, BlendFactor dst);
    void setStartColor(math::vec4);
    math::vec4 getStartColor() const;
    void setStartColor(float r, float g, float b, float a);
    void setStartColorRed(float);
    void setStartColorGreen(float);
    void setStartColorBlue(float);
    void setStartColorAlpha(float);
    void setStartColorVariance(math::vec4);
    void setStartColorVariance(float r, float g, float b, float a);
    void setStartColorVarianceRed(float);
    void setStartColorVarianceGreen(float);
    void setStartColorVarianceBlue(float);
    void setStartColorVarianceAlpha(float);
    void setEndColor(math::vec4);
    void setEndColor(float r, float g, float b, float a);
    void setEndColorRed(float);
    void setEndColorGreen(float);
    void setEndColorBlue(float);
    void setEndColorAlpha(float);
    void setEndColorVariance(math::vec4);
    void setEndColorVariance(float r, float g, float b, float a);
    void setEndColorVarianceRed(float);
    void setEndColorVarianceGreen(float);
    void setEndColorVarianceBlue(float);
    void setEndColorVarianceAlpha(float);
    void setStartSize(float);
    void setStartSizeVariance(float);
    void setEndSize(float);
    void setSendSizeVariance(float);
    void setSourcePosition(math::vec2);
    void setSourcePosition(float x, float y);
    void setSourcePositionX(float);
    void setSourcePositionY(float);
    void setSourcePositionVariance(math::vec2);
    void setSourcePositionVariance(float x, float y);
    void setSourcePositionVarianceX(float);
    void setSourcePositionVarianceY(float);
    void setStartRotation(float);
    void setStartRotationVariance(float);
    void setEndRotation(float);
    void setEndRotationVariance(float);
    void setParticleLifeTime(float);
    void setParticleLifeTimeVariance(float);
    void setTexture(w4::cref<resources::Texture>);
    void setEmitterType(ParticlesEmitterParameters::EmitterType);

    //gravity type
    void setGravity(math::vec2);
    void setGravity(float x, float y);
    void setGravityX(float);
    void setGravityY(float);
    void setStartSpeed(float);
    void setStartSpeedVariance(float);
    void setRadialAcceleration(float);
    void setRadialAccelerationVariance(float);
    void setTangentialAcceleration(float);
    void setTangentialAccelerationVariance(float);

    //radius type
    void setMinimumRadius(float);
    void setMinimumRadiusVariance(float);
    void setMaximumRadius(float);
    void setMaximumRadiusVariance(float);
    void setRotationSpeed(float);
    void setRotationSpeedVariance(float);

    // transform mode
    void setTransformMode(ParticlesEmitterParameters::TransformMode);

    void onUpdate(float dt) override;
    void onRender(const IRenderPass&) override;

    void buildMaterialInst();

    //tool
    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination) override;
private:
    class Pimpl;
    uptr<Pimpl> m_d;

    std::optional<uint32_t> m_updateHandle;
    void registerUpdate();
    void unregisterUpdate();
};

} //namespace w4::render::particles
