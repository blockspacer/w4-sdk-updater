#pragma once

namespace w4::render {

enum class BlendFactor : int32_t
{
    ZERO = 0,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA,
    SRC_ALPHA_SATURATE
};

enum class RenderVariantFlag : uint32_t
{
    STATIC      = 0,
    SKINNED     = 1 << 0,
    SHADOWS     = 1 << 1,
    MAX_VALUE   = 1 << 2
};

#define W4_MAX_DIRECTIONAL_LIGHTS 1
#define W4_MAX_POINT_LIGHTS 8
#define W4_MAX_SPOT_LIGHTS 4

struct BlendFunc
{
    BlendFactor src;
    BlendFactor dst;
};

}