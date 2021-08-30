#pragma once

#include "W4Math.h"
#include "Texture.h"
#include "Cubemap.h"

namespace w4::resources {

enum class ParamType
{
    INVALID,
    INT,
    FLOAT,
    BOOL,
    VEC2,
    VEC3,
    VEC4,
    MAT4,
    TEXTURE,
    CUBEMAP
};

template<ParamType enumType>
struct ParamTypeMappingHelper
{
    static constexpr ParamType type() { return enumType;}
};

template<typename T>
struct ParamTypeMapping
{
    static constexpr ParamType type() { return ParamType::INVALID;}
};
template<> struct ParamTypeMapping<int> : ParamTypeMappingHelper<ParamType::INT> {};
template<> struct ParamTypeMapping<float> : ParamTypeMappingHelper<ParamType::FLOAT> {};
template<> struct ParamTypeMapping<bool> : ParamTypeMappingHelper<ParamType::BOOL> {};
template<> struct ParamTypeMapping<math::vec2> : ParamTypeMappingHelper<ParamType::VEC2> {};
template<> struct ParamTypeMapping<math::vec3> : ParamTypeMappingHelper<ParamType::VEC3> {};
template<> struct ParamTypeMapping<math::vec4> : ParamTypeMappingHelper<ParamType::VEC4> {};
template<> struct ParamTypeMapping<math::mat4> : ParamTypeMappingHelper<ParamType:: MAT4> {};
template<> struct ParamTypeMapping<sptr<resources::Texture>> : ParamTypeMappingHelper<ParamType::TEXTURE> {};
template<> struct ParamTypeMapping<sptr<resources::Cubemap>> : ParamTypeMappingHelper<ParamType::CUBEMAP> {};

struct ParamData
{
    ParamType type;

    union U {
        int i;
        float f;
        bool b;
        math::vec2 v2;
        math::vec3 v3;
        math::vec4 v4;
        math::mat4 m4;

        U() {}
        U(int v) : i(v) {}
        U(float v) : f(v) {}
        U(bool v) : b(v) {}
        U(const math::vec2& v) : v2(v) {}
        U(const math::vec3& v) : v3(v) {}
        U(const math::vec4& v) : v4(v) {}
        U(const math::mat4& v) : m4(v) {}
    } data;
    sptr<Texture> texture;
    sptr<Cubemap> cubemap;

    ParamData() {}

    template<typename T>
    ParamData(const T& v);

    template<typename T>
    const T & get() const;
};

template<typename T>
ParamData::ParamData(const T& v)
    : type(ParamTypeMapping<T>::type())
    , data(v)
{}

template<>
inline ParamData::ParamData(cref<resources::Texture> v)
    : type(ParamTypeMapping<sptr<resources::Texture>>::type())
    , texture(v)
{}

template<>
inline ParamData::ParamData(cref<resources::Cubemap> v)
    : type(ParamTypeMapping<sptr<resources::Cubemap>>::type())
    , cubemap(v)
{}

template<> inline const int& ParamData::get() const { return data.i; }
template<> inline const float& ParamData::get() const { return data.f; }
template<> inline const bool& ParamData::get() const { return data.b; }
template<> inline const math::vec2& ParamData::get() const { return data.v2; }
template<> inline const math::vec3& ParamData::get() const { return data.v3; }
template<> inline const math::vec4& ParamData::get() const { return data.v4; }
template<> inline const math::mat4& ParamData::get() const { return data.m4; }
template<> inline cref<Texture> ParamData::get() const { return texture; }
template<> inline cref<Cubemap> ParamData::get() const { return cubemap; }

} //namespace w4::resources