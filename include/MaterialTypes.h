#pragma once

#include "Material.h"

namespace w4::resources {

    class MaterialTypeProcessor
    {
    public:
        virtual ~MaterialTypeProcessor() {}

        virtual void initilizeDefault(MaterialInst& inst, const std::string& name) = 0;
        virtual void initilizeFromData(MaterialInst& inst, const std::string& name, const ParamData& data) = 0;
    };

    template<typename T, T ParamData::U::* Field>
    class ParamUnionTypeProcessor : public MaterialTypeProcessor
    {
    public:
        void initilizeDefault(MaterialInst& inst, const std::string& name) override
        {
            inst.setParam(name, T());
        }

        void initilizeFromData(MaterialInst& inst, const std::string& name, const ParamData& data) override
        {
            inst.setParam(name, data.data.*Field);
        }
    };

    class IntTypeProcessor   : public ParamUnionTypeProcessor<int, &ParamData::U::i> {};
    class FloatTypeProcessor : public ParamUnionTypeProcessor<float, &ParamData::U::f> {};
    class BoolTypeProcessor  : public ParamUnionTypeProcessor<bool, &ParamData::U::b> {};
    class Vec2TypeProcessor  : public ParamUnionTypeProcessor<math::vec2, &ParamData::U::v2> {};
    class Vec3TypeProcessor  : public ParamUnionTypeProcessor<math::vec3, &ParamData::U::v3> {};
    class Vec4TypeProcessor  : public ParamUnionTypeProcessor<math::vec4, &ParamData::U::v4> {};
    class Mat4TypeProcessor  : public ParamUnionTypeProcessor<math::mat4, &ParamData::U::m4> {};

    class TextureTypeProcessor : public MaterialTypeProcessor
    {
    public:
        void initilizeDefault(MaterialInst& inst, const std::string& name) override
        {
            inst.setParam(name, resources::Texture::getDefault());
        }

        void initilizeFromData(MaterialInst& inst, const std::string& name, const ParamData& data) override
        {
            inst.setParam(name, data.texture);
        }
    };

    class CubemapTypeProcessor : public MaterialTypeProcessor
    {
    public:
        void initilizeDefault(MaterialInst& inst, const std::string& name) override
        {
            inst.setParam(name, resources::Cubemap::getDefault());
        }

        void initilizeFromData(MaterialInst& inst, const std::string& name, const ParamData& data) override
        {
            inst.setParam(name, data.cubemap);
        }
    };

    static const std::unordered_map<ParamType, sptr<MaterialTypeProcessor>>& getTypeProcessors()
    {
        static std::unordered_map<ParamType, sptr<MaterialTypeProcessor>> materialTypesProcessors =
                {
                        {ParamType::INT,     make::sptr<IntTypeProcessor>()},
                        {ParamType::FLOAT,   make::sptr<FloatTypeProcessor>()},
                        {ParamType::BOOL,    make::sptr<BoolTypeProcessor>()},
                        {ParamType::VEC2,    make::sptr<Vec2TypeProcessor>()},
                        {ParamType::VEC3,    make::sptr<Vec3TypeProcessor>()},
                        {ParamType::VEC4,    make::sptr<Vec4TypeProcessor>()},
                        {ParamType::MAT4,    make::sptr<Mat4TypeProcessor>()},
                        {ParamType::TEXTURE, make::sptr<TextureTypeProcessor>()},
                        {ParamType::CUBEMAP, make::sptr<CubemapTypeProcessor>()}
                };
        return materialTypesProcessors;
    }
}
