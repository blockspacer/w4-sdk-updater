#pragma once
#include <optional>

#include "FatalError.h"
#include "Cache.h"
#include "W4Math.h"
#include "Utils.h"
#include "IOuterID.h"
#include "Texture.h"
#include "Cubemap.h"
#include "RenderCommon.h"
#include "VFS.h"
#include "Shader.h"
#include "MaterialParameters.h"

namespace w4::resources {

class IVerticesBuffer;

enum class PrimitiveType
{
    POINTS,
    LINE_STRIP,
    LINE_LOOP,
    LINES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    TRIANGLES
};

struct MaterialData
{
    PrimitiveType primitiveType;
    std::string vertexShader;
    std::string fragmentShader;
    std::optional<render::BlendFunc> blendFunc;
    std::unordered_map<std::string, ParamData> params;
    std::vector<std::string> defines;
};

/*struct MaterialInstData
{
    std::string material;
    ResourceLoadDescr loadDescr;
    std::unordered_map<std::string, ParamData> params;
};*/

class Material;

class MaterialInst;
class MaterialTypeProcessor;

class Material
        : public Resource
        , public core::Cache<Material, sptr<Material>, ResourceLoadDescr>
        , public core::IOuterManaged
{
    W4_OBJECT(Material, Resource);
public:
    static sptr<Material> load(const ResourceLoadDescr& descr);
    static sptr<Material> parse(const ResourceLoadDescr& descr,
                                const MaterialData& materialContent,
                                const std::vector<std::string>& defines = {},
                                uint32_t flagsToIgnore = 0);

public:
    static sptr<Material> getDefault();
    static sptr<Material> getDefaultSkybox();
    static sptr<Material> getDefaultBackground();
    static sptr<Material> getDefaultLambert();
    static sptr<Material> getDefaultBlinn();

    static sptr<Material> getDefaultSpotLightShadowMap();
    static sptr<Material> getDefaultDirectionalShadowMap();

public:
    struct ParamIDType
    {
        ParamType type;
        size_t size;

        core::OuterID::ndxType id;
        size_t offset;
    };

    using ParamsIDMapping = std::unordered_map<std::string, ParamIDType >;
    using IDParamsMapping = std::unordered_map<decltype(Material::ParamIDType::id), std::string >;

public:
    bool getParamId(const std::string& name, ParamIDType& res) const;
    std::string getParamName(decltype(Material::ParamIDType::id) id) const;
    size_t paramsCount() const;
    size_t paramsByteSize() const;

    PrimitiveType getPrimitiveType() const;

    sptr<MaterialInst> createInstance(uint64_t resourceUid = 0);

    void outerCreator() override;
    void outerDeleter() override;

    //bool isCompatible(cref<IVerticesBuffer>) const;

#if defined(W4_ENABLE_SERIALIZERS)
    MaterialData getMaterialData() const;
#endif
    void foreachParam(ParamType, std::function<void(const std::string& idType)>);

private:
    friend class MaterialInst;
    Material(const ResourceLoadDescr& descr);
    Material(const std::string& name, const ResourceLoadDescr& descr);

    size_t registerParam(const std::string& name, ParamIDType&& param);
    sptr<MaterialInst>  getInternalDefaultValues();

    static sptr<Material> loadPath(const ResourceLoadDescr& descr);
    static sptr<Material> loadGenerator(const ResourceLoadDescr& descr);

    sptr<MaterialInst> createInstance(const ResourceLoadDescr& descr);

    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination) override;

#if defined(__EMSCRIPTEN__)
    void registerShaderProgram(cref<ShaderProgram> shaderProgram, uint32_t flag);
#endif

    static const std::string& getDefaultVertexTemplate();
    static const std::string& getDefaultVertexMain();

    static const std::string& getDefaultFragmentTemplate();
    static const std::string& getDefaultFragmentMain();

private:
    ParamsIDMapping m_paramIDMapping;
    IDParamsMapping m_IDParamMapping;

    std::unordered_map<uint32_t, sptr<ShaderProgram>> m_shaderPrograms;
    PrimitiveType m_primitiveType;
    sptr<MaterialInst> m_defaultValues;
    size_t m_userParamsSize = 0;
    uint8_t m_currentParamId = 0;

#if defined(W4_ENABLE_SERIALIZERS)
    MaterialData m_data;
#endif
};

sptr<Material> operator"" _material(const char*, literal_int);

} //namespace w4::resources
