#pragma once

#include <vector>
#include <unordered_map>
#include "external/variant.hpp"
#include "external/variant_visitor.hpp"
#include "W4Math.h"

namespace w4::variant
{
    enum class VariantType
    {
        NoneType,
        Int,
        Float,
        String,
        Vec2,
        Vec3,
        Vec4,
        Mat4,
        Array,
        Object
    };

    struct NoneType{
        NoneType(){}
        bool operator == (const NoneType& rVal) const { return true; };
        bool operator != (const NoneType& rVal) const { return false;};
    };

    struct VariantArray;
    struct VariantObject;

    typedef mapbox::util::variant<
            NoneType,
            float,
            int,
            std::string,
            math::vec2,
            math::vec3,
            math::vec4,
            math::mat4,
            mapbox::util::recursive_wrapper<VariantArray>,
            mapbox::util::recursive_wrapper<VariantObject>
    > Variant;

    struct VariantArray : std::vector<Variant>
    {};

    struct VariantObject : std::unordered_map<std::string, Variant>
    {};

    struct GetType
    {
        VariantType operator() (const NoneType&)      const { return VariantType::NoneType; }
        VariantType operator() (const float&)         const { return VariantType::Float; }
        VariantType operator() (const int&)           const { return VariantType::Int; }
        VariantType operator() (const math::vec2&)    const { return VariantType::Vec2; }
        VariantType operator() (const math::vec3&)    const { return VariantType::Vec3; }
        VariantType operator() (const math::vec4&)    const { return VariantType::Vec4; }
        VariantType operator() (const math::mat4&)    const { return VariantType::Mat4; }
        VariantType operator() (const std::string&)   const { return VariantType::String; }
        VariantType operator() (const VariantArray&)  const { return VariantType::Array; }
        VariantType operator() (const VariantObject&) const { return VariantType::Object; }

        GetType() = default;
    };

    inline VariantType typeOfVariant(const Variant& v)
    {
        return mapbox::util::apply_visitor(GetType(), v);
    }

    inline Variant getFromObjectOrNone(const VariantObject& obj, const std::string& key)
    {
        auto it = obj.find(key);
        if (it == obj.end())
        {
            return NoneType();
        }

        return it->second;
    }
}
