#pragma once

#include "Material.h"

namespace w4::resources {

#include "impl/MaterialParametersPackDecl.inl"

#define PARAMSLIST int, float, bool, math::vec2, math::vec3, math::vec4, math::mat4, sptr<resources::Texture>, sptr<resources::Cubemap>

class MaterialInst
    : public Resource
    , public core::Cache<MaterialInst, sptr<MaterialInst>, ResourceLoadDescr, core::CopyCreator<MaterialInst, sptr<MaterialInst>>>
    , public MaterialParameters<PARAMSLIST>
    , public core::IOuterManaged
{
W4_OBJECT(MaterialInst, Resource)
public:
    static sptr<MaterialInst> load(const ResourceLoadDescr& uri);

    struct predefined
    {
        static sptr<MaterialInst> mesh();
        static sptr<MaterialInst> lambert();
        static sptr<MaterialInst> blinn();
    };

public:
    MaterialInst(const ResourceLoadDescr& descr, cref<Material>, const std::unordered_map<std::string, ParamData>& params);

    cref<Material> getMaterial() const;

    void outerCreator() override;
    void outerDeleter() override;

    void onRender();

    uint8_t* getParamsRawData();
    const uint8_t* getParamsRawData() const;

    void enableBlending(bool state);
    bool isBlending() const;
    void setBlendFunc(render::BlendFactor src, render::BlendFactor dst);
    render::BlendFactor getSrcBlendFactor() const;
    render::BlendFactor getDstBlendFactor() const;

    std::unordered_map<std::string, ParamData> saveParams() const;

    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination) override;

private:
    static sptr<MaterialInst> loadFile(const ResourceLoadDescr& descr);

    explicit MaterialInst(const sptr<Material>& mat, uint64_t resourceUid = 0);
    explicit MaterialInst(const MaterialInst&, uint64_t resourceUid = 0);
    explicit MaterialInst(const MaterialInst& , const ResourceLoadDescr& descr);

    friend class Material;
    friend struct core::CopyCreator<MaterialInst, sptr<MaterialInst>>;

    void initilizeDefaultParams();

    void initBlending(const render::BlendFunc& data);
    void initParams(const std::unordered_map<std::string, ParamData>& data);
    void updateBlending();

private:
    sptr<Material> m_material;
    std::vector<uint8_t> m_paramsData;

    bool m_isBlendingEnabled = false;
    std::optional<render::BlendFactor> m_srcBlend;
    std::optional<render::BlendFactor> m_dstBlend;
};

#include "impl/MaterialParametersPackImpl.inl"

} //namespace w4::resources