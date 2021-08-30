#pragma once

#include <cstdlib>
#include <type_traits>
#include "FatalError.h"
#include "W4Math.h"
#include "Variant.h"
#include "Track.h"
#include "GUI.h"

template<typename T>
const char* JsonExceptionWrapper(T e)
{
    return e.what();
}

template<typename T>
const char* JsonExceptionWrapper(T* e)
{
    return e->what();
}

#if defined(__EMSCRIPTEN__)
    #define JSON_THROW_USER(exception) FATAL_ERROR("nlohmann exception: %s", JsonExceptionWrapper(exception)); std::abort();
    #define JSON_TRY_USER if(true)
    #define JSON_CATCH_USER(exception) if(false)
    #define JSON_INTERNAL_CATCH_USER(exception) if(false)
#endif


#define W4_NLOHMANN_JSON_SERIALIZE_ENUM(ENUM_TYPE, ...) \
template<> \
struct adl_serializer<ENUM_TYPE> \
{ \
    template<typename BasicJsonType>                                                            \
    static void to_json(BasicJsonType& j, const ENUM_TYPE& e)                                   \
    {                                                                                           \
        static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE " must be an enum!");          \
        static const std::pair<ENUM_TYPE, BasicJsonType> m[] = __VA_ARGS__;                     \
        auto it = std::find_if(std::begin(m), std::end(m),                                      \
                               [e](const std::pair<ENUM_TYPE, BasicJsonType>& ej_pair) -> bool  \
        {                                                                                       \
            return ej_pair.first == e;                                                          \
        });                                                                                     \
        j = ((it != std::end(m)) ? it : std::begin(m))->second;                                 \
    }                                                                                           \
    template<typename BasicJsonType>                                                            \
    static void from_json(const BasicJsonType& j, ENUM_TYPE& e)                                 \
    {                                                                                           \
        static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE " must be an enum!");          \
        static const std::pair<ENUM_TYPE, BasicJsonType> m[] = __VA_ARGS__;                     \
        auto it = std::find_if(std::begin(m), std::end(m),                                      \
                               [&j](const std::pair<ENUM_TYPE, BasicJsonType>& ej_pair) -> bool \
        {                                                                                       \
            return ej_pair.second == j;                                                         \
        });                                                                                     \
        e = ((it != std::end(m)) ? it : std::begin(m))->first;                                  \
    }\
};



#include "external/json.hpp"

namespace nlohmann {

W4_NLOHMANN_JSON_SERIALIZE_ENUM(w4::variant::VariantType, {
    { w4::variant::VariantType::NoneType, "NoneType"},
    { w4::variant::VariantType::Int,      "Int"},
    { w4::variant::VariantType::Float,    "Float"},
    { w4::variant::VariantType::String,   "String"},
    { w4::variant::VariantType::Vec2,     "Vec2"},
    { w4::variant::VariantType::Vec3,     "Vec3"},
    { w4::variant::VariantType::Vec4,     "Vec4"},
    { w4::variant::VariantType::Mat4,     "Mat4"},
    { w4::variant::VariantType::Array,    "Array"},
    { w4::variant::VariantType::Object,   "Object"}
});


W4_NLOHMANN_JSON_SERIALIZE_ENUM(w4::gui::VerticalAlign, {
    { w4::gui::VerticalAlign::Center, "Center" },
    { w4::gui::VerticalAlign::Top, "Top" },
    { w4::gui::VerticalAlign::Bottom, "Bottom" }
});


W4_NLOHMANN_JSON_SERIALIZE_ENUM(w4::gui::HorizontalAlign, {
    { w4::gui::HorizontalAlign::Center, "Center" },
    { w4::gui::HorizontalAlign::Left, "Left" },
    { w4::gui::HorizontalAlign::Right, "Right" }
});


W4_NLOHMANN_JSON_SERIALIZE_ENUM(w4::gui::SizePolicy, {
    { w4::gui::SizePolicy::Auto, "Auto" },
    { w4::gui::SizePolicy::Fixed, "Fixed" },

});

template<>
struct adl_serializer<w4::math::size>
{
    static void to_json(json& j, const w4::math::size &v)
    {
        j = json
                {
                        {"w", v.w},
                        {"h", v.h}
                };
    }

    static void from_json(const json& j, w4::math::size& v)
    {
        j.at("w").get_to(v.w);
        j.at("h").get_to(v.h);
    }
};

template<>
struct adl_serializer<w4::math::ivec2>
{
    static void to_json(json& j, const w4::math::ivec2 &v)
    {
        j["x"] = v.x;
        j["y"] = v.y;
    }

    static void from_json(const json& j, w4::math::ivec2& v)
    {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
    }
};

template<>
struct adl_serializer<w4::math::ivec3>
{
    static void to_json(json& j, const w4::math::ivec3 &v)
    {
        j["x"] = v.x;
        j["y"] = v.y;
        j["z"] = v.z;
    }

    static void from_json(const json& j, w4::math::ivec3& v)
    {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
        j.at("z").get_to(v.z);
    }
};

template<>
struct adl_serializer<w4::math::ivec4>
{
    static void to_json(json& j, const w4::math::ivec4 &v)
    {
        j["x"] = v.x;
        j["y"] = v.y;
        j["z"] = v.z;
        j["w"] = v.w;
    }

    static void from_json(const json& j, w4::math::ivec4& v)
    {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
        j.at("z").get_to(v.z);
        j.at("w").get_to(v.w);
    }
};

template<>
struct adl_serializer<w4::math::vec2>
{
    static void to_json(json& j, const w4::math::vec2 &v)
    {
        j = json
                {
                        {"x", v.x},
                        {"y", v.y}
                };
    }

    static void from_json(const json& j, w4::math::vec2& v)
    {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
    }
};

template<>
struct adl_serializer<w4::math::vec3>
{
    static void to_json(json& j, const w4::math::vec3 &v)
    {
        j = json
                {
                        {"x", v.x},
                        {"y", v.y},
                        {"z", v.z}
                };
    }

    static void from_json(const json& j, w4::math::vec3& v)
    {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
        j.at("z").get_to(v.z);
    }
};

template<>
struct adl_serializer<w4::math::vec4>
{
    static void to_json(json& j, const w4::math::vec4 &v)
    {
        j = json
                {
                        {"x", v.x},
                        {"y", v.y},
                        {"z", v.z},
                        {"w", v.w},
                };
    }

    static void from_json(const json& j, w4::math::vec4& v)
    {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
        j.at("z").get_to(v.z);
        j.at("w").get_to(v.w);
    }
};

template<>
struct adl_serializer<w4::math::mat4>
{
    static void to_json(json& j, const w4::math::mat4 &v)
    {
        j = json
                {
                        {"xx", v.xx},
                        {"xy", v.xy},
                        {"xz", v.xz},
                        {"xw", v.xw},

                        {"yx", v.yx},
                        {"yy", v.yy},
                        {"yz", v.yz},
                        {"yw", v.yw},

                        {"zx", v.zx},
                        {"zy", v.zy},
                        {"zz", v.zz},
                        {"zw", v.zw},

                        {"wx", v.wx},
                        {"wy", v.wy},
                        {"wz", v.wz},
                        {"ww", v.ww},
                };
    }

    static void from_json(const json& j, w4::math::mat4& v)
    {
        if(!j.contains("xx"))
        {
            W4_LOG_INFO("NOTHING %s", j.dump(4).data());
        }
        j.at("xx").get_to(v.xx);
        j.at("xy").get_to(v.xy);
        j.at("xz").get_to(v.xz);
        j.at("xw").get_to(v.xw);

        j.at("yx").get_to(v.yx);
        j.at("yy").get_to(v.yy);
        j.at("yz").get_to(v.yz);
        j.at("yw").get_to(v.yw);

        j.at("zx").get_to(v.zx);
        j.at("zy").get_to(v.zy);
        j.at("zz").get_to(v.zz);
        j.at("zw").get_to(v.zw);

        j.at("wx").get_to(v.wx);
        j.at("wy").get_to(v.wy);
        j.at("wz").get_to(v.wz);
        j.at("ww").get_to(v.ww);
    }
};

template<>
struct adl_serializer<w4::math::mat3>
{
    static void to_json(json& j, const w4::math::mat3 &v)
    {
        j = json
                {
                        {"xx", v.xx},
                        {"xy", v.xy},
                        {"xz", v.xz},

                        {"yx", v.yx},
                        {"yy", v.yy},
                        {"yz", v.yz},

                        {"zx", v.zx},
                        {"zy", v.zy},
                        {"zz", v.zz}
                };
    }

    static void from_json(const json& j, w4::math::mat3& v)
    {
        j.at("xx").get_to(v.xx);
        j.at("xy").get_to(v.xy);
        j.at("xz").get_to(v.xz);

        j.at("yx").get_to(v.yx);
        j.at("yy").get_to(v.yy);
        j.at("yz").get_to(v.yz);

        j.at("zx").get_to(v.zx);
        j.at("zy").get_to(v.zy);
        j.at("zz").get_to(v.zz);
    }
};

template<>
struct adl_serializer<w4::math::Rotator>
{
    static void to_json(json& j, const w4::math::Rotator &r)
    {
        j = json
                {
                        {"x", r.quaternion.x},
                        {"y", r.quaternion.y},
                        {"z", r.quaternion.z},
                        {"w", r.quaternion.w}
                };
    }

    static void from_json(const json& j, w4::math::Rotator& r)
    {
        j.at("x").get_to(r.quaternion.x);
        j.at("y").get_to(r.quaternion.y);
        j.at("z").get_to(r.quaternion.z);
        j.at("w").get_to(r.quaternion.w);
    }
};

template<>
struct adl_serializer<w4::math::Transform>
{
    static void to_json(json& j, const w4::math::Transform &t)
    {
        j = json
                {
                        {"translation", t.translation()},
                        {"rotation", t.rotation()},
                        {"scale", t.scale()}
                };
    }

    static void from_json(const json& j, w4::math::Transform& t)
    {
        j.at("translation").get_to(t.translation());
        j.at("rotation").get_to(t.rotation());
        j.at("scale").get_to(t.scale());
    }
};

template<>
struct adl_serializer<w4::variant::Variant>
{
    static void to_json(json& j, const w4::variant::Variant &v)
    {
        const auto typeOfVariant = w4::variant::typeOfVariant(v);
        j["type"] = typeOfVariant;
        switch(typeOfVariant)
        {
        case w4::variant::VariantType::NoneType:
            break;
        case w4::variant::VariantType::Int:
            j["value"] = v.get<int>();
            break;
        case w4::variant::VariantType::Float:
            j["value"] = v.get<float>();
            break;
        case w4::variant::VariantType::String:
            j["value"] = v.get<std::string>();
            break;
        case w4::variant::VariantType::Vec2:
            j["value"] = v.get<w4::math::vec2>();
            break;
        case w4::variant::VariantType::Vec3:
            j["value"] = v.get<w4::math::vec3>();
            break;
        case w4::variant::VariantType::Vec4:
            j["value"] = v.get<w4::math::vec4>();
            break;
        case w4::variant::VariantType::Mat4:
            j["value"] = v.get<w4::math::mat4>();
            break;
        case w4::variant::VariantType::Array:
        {
            const auto &array = v.get<w4::variant::VariantArray>();
            auto& value = j["value"];
            size_t idx = 0;
            for (const auto &elem: array)
            {
                value[std::to_string(idx)] = elem;
            }
            break;
        }
        case w4::variant::VariantType::Object:
        {
            const auto &object = v.get<w4::variant::VariantObject>();
            auto& value = j["value"];
            for (const auto &elem: object)
            {
                value[elem.first] = elem.second;
            }
            break;
        }
        }
    }

    static void from_json(const json& j, w4::variant::Variant &v)
    {
        auto it = j.find("type");
        if (it != j.end()) {
            w4::variant::VariantType typeOfVariant = j["type"];
            switch (typeOfVariant) {
                case w4::variant::VariantType::NoneType:
                    v = w4::variant::NoneType();
                    break;
                case w4::variant::VariantType::Int:
                    v = j["value"].get<int>();
                    break;
                case w4::variant::VariantType::Float:
                    v = j["value"].get<float>();
                    break;
                case w4::variant::VariantType::String:
                    v = j["value"].get<std::string>();
                    break;
                case w4::variant::VariantType::Vec2:
                    v = j["value"].get<w4::math::vec2>();
                    break;
                case w4::variant::VariantType::Vec3:
                    v = j["value"].get<w4::math::vec3>();
                    break;
                case w4::variant::VariantType::Vec4:
                    v = j["value"].get<w4::math::vec4>();
                    break;
                case w4::variant::VariantType::Mat4:
                    v = j["value"].get<w4::math::mat4>();
                    break;
                case w4::variant::VariantType::Array: {
                    v = w4::variant::VariantArray();
                    const auto &value = j["value"];
                    auto &array = v.get<w4::variant::VariantArray>();
                    size_t idx = 0;
                    while (true) {
                        const auto iter = value.find(std::to_string(idx));
                        if (iter == value.end()) {
                            break;
                        }
                        array.push_back(*iter);
                    }
                    break;
                }
                case w4::variant::VariantType::Object: {
                    v = w4::variant::VariantObject();
                    const auto &value = j["value"];
                    auto &object = v.get<w4::variant::VariantObject>();
                    for (auto iter = value.begin(); iter != value.end(); ++iter) {
                        object[iter.key()] = iter.value();
                    }
                    break;
                }
            }
        } else {
            switch (j.type())
            {
                case detail::value_t::null :
                {
                    v = w4::variant::NoneType();
                    break;
                }
                case detail::value_t::object :
                {
                    v = w4::variant::VariantObject();
                    auto &object = v.get<w4::variant::VariantObject>();
                    for (auto iter = j.begin(); iter != j.end(); ++iter) {
                        object[iter.key()] = iter.value();
                    }
                    break;
                }
                case detail::value_t::array :
                {
                    v = w4::variant::VariantArray();
                    auto &array = v.get<w4::variant::VariantArray>();
                    size_t idx = 0;
                    while (true) {
                        const auto iter = j.find(std::to_string(idx));
                        if (iter == j.end()) {
                            break;
                        }
                        array.push_back(*iter);
                    }
                    break;
                }
                case detail::value_t::string :
                {
                    v = j.get<std::string>();
                    break;
                }
                case detail::value_t::boolean :
                {
                    v = static_cast<int>(j.get<bool>());
                    break;
                }
                case detail::value_t::number_integer :
                {
                    v = j.get<int>();
                    break;
                }
                case detail::value_t::number_unsigned :
                {
                    v = j.get<int>();
                    break;
                }
                case detail::value_t::number_float :
                {
                    v = j.get<float>();
                    break;
                }
                default:
                {
                    v = w4::variant::NoneType();
                }
            }
        }
    }
};

template<typename T>
struct adl_serializer<std::optional<T>> {
    static void to_json(json& j, const std::optional<T>& obj)
    {
        if (obj.has_value())
        {
            adl_serializer<T>::to_json(j, *obj);
        } else {
            j = nullptr;
        }
    }

    static void from_json(const json& j, std::optional<T>& obj)
    {
        if (j.is_null())
        {
            obj = std::nullopt;
            return;
        }
        T value;
        nlohmann::adl_serializer<T>::from_json(j, value);
        obj = std::make_optional(value);
    }
};

template<typename T>
struct adl_serializer<w4::sptr<T>> {
    static void to_json(json& j, const w4::sptr<T>& obj)
    {
        if (obj.get())
        {
            adl_serializer<T>::to_json(j, *obj);
        } else {
            j = nullptr;
        }
    }

    static void from_json(const json& j, w4::sptr<T>& obj)
    {
        auto res = w4::make::sptr<T>(T::defaultName);
        nlohmann::adl_serializer<T>::from_json(j, *res);
        obj.swap(res);
    }
};

W4_NLOHMANN_JSON_SERIALIZE_ENUM(w4::resources::InterpType, {
    { w4::resources::InterpType::CONST, "const" },
    { w4::resources::InterpType::LERP, "lerp" },
    { w4::resources::InterpType::HERMIT, "hermit" },
    { w4::resources::InterpType::CUBIC, "cubic" }
});

template<typename T>
struct adl_serializer<w4::resources::TrackValue<T>>
{
    static void to_json(json& j, const w4::resources::TrackValue<T>& obj)
    {
        j["time"] = obj.time;
        j["value"] = obj.value;
        j["interp"] = obj.interp;
        if(obj.interp == w4::resources::InterpType::HERMIT)
        {
            j["tangents"] = obj.tangents;
        }
    }

    static void from_json(const json& j, w4::resources::TrackValue<T>& obj)
    {
        obj.time = j["time"];
        obj.value = j["value"];
        obj.interp = j["interp"];
        if(obj.interp == w4::resources::InterpType::HERMIT)
        {
            j["tangents"].get_to(obj.tangents);
        }
    }
};

template<typename T>
struct adl_serializer<w4::resources::Track<T>>
{
    static void to_json(json& j, const w4::resources::Track<T>& obj)
    {
        j["value_type"] = obj.getSubType();
        j["values"] = obj.getValues();
    }

    static void from_json(const json& j, w4::resources::Track<T>& obj)
    {
        obj.setSubType(j["value_type"]);
        typename w4::resources::Track<T>::Values values;
        adl_serializer<typename w4::resources::Track<T>::Values>::from_json(j["values"], values);
        for (auto& v : values)
        {
            obj.addValue(v);
        }
    }
};

#define DECL_SERIALIZE_TRACK(SUFFIX) template<> struct adl_serializer<w4::resources::Track##SUFFIX>: adl_serializer<w4::resources::Track##SUFFIX::Super>{};
DECL_SERIALIZE_TRACK(Float);
DECL_SERIALIZE_TRACK(Vec3);
DECL_SERIALIZE_TRACK(Rotator);
#undef DECL_SERIALIZE_TRACK

template<typename T>
void loadValue(const nlohmann::json& src, T& value) {
    adl_serializer<T>::from_json(src, value);
}

template<typename T>
static void loadValueFromFile(const w4::sptr<w4::filesystem::Stream>& file, T& value) {
    nlohmann::json j = nlohmann::json::parse(file->data(), file->data() + file->size());
    loadValue(j, value);
}

template <typename T>
void LookUp(const nlohmann::json& j, const std::string& key, T& result)
{
    auto it = j.find(key); if (it != j.end()) result = *it;
}

}
