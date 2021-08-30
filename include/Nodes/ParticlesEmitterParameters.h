#pragma once

#include "W4Math.h"
#include "Texture.h"
#include "RenderCommon.h"

namespace w4::render {

struct ParticlesEmitterParameters
{
    enum class EmitterType: uint8_t
    {
        GRAVITY = 0,
        RADIUS = 1
    };

    enum class TransformMode : uint8_t
    {
        EMITTER,
        PARTICLE
    };

    template<class T>
    struct Param
    {
        using Type = T;

        T value;
        T variance;
    };

    std::uint32_t maxParticles;

    Param<float> angle;
    float duration;
    int32_t yCoordsFlipped;
    BlendFunc blendFunc = {BlendFactor::ONE, BlendFactor::ONE_MINUS_SRC_ALPHA};
    Param<math::vec4> startColor;
    Param<math::vec4> endColor;
    Param<float> startSize;
    Param<float> endSize;
    Param<math::vec2> sourcePosition;

    Param<float> startRotation;
    Param<float> endRotation;

    Param<float> particleLifeSpan;

    w4::sptr<resources::Texture> texture;

    EmitterType emitterType = EmitterType::GRAVITY;

    struct
    {
        math::vec2 gravity;
        Param<float> startSpeed;
        Param<float> radialAcceleration;
        Param<float> tangentialAcceleration;
        bool rotationIsDir;
    }gravityParameters;

    struct
    {
        Param<float> minRadius;
        Param<float> maxRadius;
        Param<float> rotatePerSecond;
    }radiusParameters;

    TransformMode transformMode = TransformMode::EMITTER;
};

} //namespace w4::resources
