#pragma once

#include "W4Math.h"
#include "IOuterID.h"
#include "RenderCommon.h"
#include "PodGenerator.h"
#include "Nodes/SkinnedMesh.h"

#define TEXTURE_SLOTS 6
#define CUBEMAPS_SLOTS 2

using TextureOuterId = w4::core::OuterID::ndxType;

POD_STRUCT(FRAME_DATA,
           POD_FIELD(w4::math::vec4, clearColor)
           POD_FIELD(int           , clearMask)

           POD_FIELD(float         , w4_u_time)
           POD_FIELD(w4::math::mat4, w4_u_projection)
           POD_FIELD(w4::math::mat4, w4_u_view)
           POD_FIELD(w4::math::mat4, w4_u_projectionView)
           POD_FIELD(w4::math::vec3, w4_u_eyePosition)
           POD_FIELD(w4::math::vec2, w4_u_resolution)
           POD_FIELD(w4::math::vec2, w4_u_nearFar)

           // Directional Light
           POD_ARRAY(w4::math::vec3, w4_u_directionalLightColor, W4_MAX_DIRECTIONAL_LIGHTS)
           POD_ARRAY(w4::math::vec3, w4_u_directionalLightDirection, W4_MAX_DIRECTIONAL_LIGHTS)
           POD_ARRAY(w4::math::mat4, w4_u_directionalLightProjectionView, W4_MAX_DIRECTIONAL_LIGHTS)

           //Point lights
           POD_ARRAY(w4::math::vec3, w4_u_pointLightPosition, W4_MAX_POINT_LIGHTS)
           POD_ARRAY(w4::math::vec4, w4_u_pointLightColor, W4_MAX_POINT_LIGHTS)

           //Spot lights
           POD_ARRAY(w4::math::vec4, w4_u_spotLightPosition, W4_MAX_SPOT_LIGHTS)
           POD_ARRAY(w4::math::vec4, w4_u_spotLightColor, W4_MAX_SPOT_LIGHTS)
           POD_ARRAY(w4::math::mat4, w4_u_spotLightProjectionView, W4_MAX_SPOT_LIGHTS)
)

POD_STRUCT(FRAME_SHADOW_DATA,
           POD_FIELD(TextureOuterId, w4_u_spotLightShadowMap)
           POD_FIELD(w4::math::vec2, w4_u_spotShadowMapResolution)

           POD_FIELD(TextureOuterId, w4_u_directionalLightShadowMap)
           POD_FIELD(w4::math::vec2, w4_u_directionalShadowMapResolution)
)

POD_STRUCT(OBJ_SHADER_DATA,
           POD_FIELD(w4::math::mat4, w4_u_model)
           POD_FIELD(w4::math::mat3, w4_u_normalSpace)
           POD_ARRAY(w4::math::mat4, w4_u_bones, W4_MAX_BONES)
)

POD_STRUCT(OBJ_DATA,
           POD_FIELD(w4::core::OuterID::ndxType, surface)
           ,
           void clear()
           {
               surface = w4::core::OuterID::invalid_value;
           }
)

namespace w4::render {

struct RenderState
{
    static sptr<FRAME_DATA_Holder>        frame;
    static sptr<FRAME_SHADOW_DATA_Holder> shadowData;
    static sptr<OBJ_SHADER_DATA_Holder>   obj;
    static sptr<OBJ_DATA_Holder>          objData;

// todo:: current env objects - camera, etc...

    static void updateNativeView();
    static void clearBackBuffer();
    static void setBlending(bool);
    static void dropVao();
    static void draw();
    static void init();
    static void setDepthTest(bool);
};

#undef FRAME_DATA
#undef OBJ_DATA


}// namespace

