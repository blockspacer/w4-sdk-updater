#pragma once

#include "Cache.h"
#include "Object.h"
#include "Variant.h"

#include <unordered_set>

namespace w4::resources {

struct ResourceGenerator
{
    ResourceGenerator();
    ResourceGenerator(const std::string& type, variant::Variant params = variant::NoneType());

    const std::string& getType() const;
    const variant::Variant& getParams() const;

    bool operator==(const ResourceGenerator& other) const;

private:
    std::string m_type;
    variant::Variant m_params;
};

struct ResourceFileDescription
{
    ResourceFileDescription(const std::string& pathInAsset);
    ResourceFileDescription(std::string&& pathInAsset);
    ResourceFileDescription(const char* pathInAsset);
    ResourceFileDescription(const std::string& assetFilePath, const std::string& pathInAsset);
    ResourceFileDescription(std::string && assetFilePath, std::string && pathInAsset);
    ResourceFileDescription(const char* assetFilePath, const char* pathInAsset);

    const std::string& getAssetFilePath() const;
    const std::string& getPathInAsset() const;

    bool operator==(const ResourceFileDescription& other) const;

private:
    std::string m_assetFilePath;
    std::string m_pathInAsset;
};

enum class ResourceLoadType
{
    None,
    File,
    Generator
};

class ResourceLoadDescr
{
public:
    ResourceLoadDescr();
    ResourceLoadDescr(const char*, uint64_t resourceUid = 0);
    ResourceLoadDescr(const std::string&, uint64_t resourceUid = 0);
    ResourceLoadDescr(std::string&&, uint64_t resourceUid = 0);
    ResourceLoadDescr(const ResourceFileDescription&, uint64_t resourceUid = 0);
    ResourceLoadDescr(ResourceFileDescription&&, uint64_t resourceUid = 0);
    ResourceLoadDescr(const ResourceGenerator& generator, uint64_t resourceUid = 0);
    ResourceLoadDescr(const ResourceLoadDescr& other);
    ~ResourceLoadDescr();

    uint64_t getResourceUid() const;
    ResourceLoadType getLoadType() const;
    cref<ResourceGenerator> getGenerator() const;
    cref<ResourceFileDescription> getFileDescription() const;
    const std::string& getName() const;

    bool operator==(const ResourceLoadDescr& other) const;

private:
    friend class Resource;
    ResourceLoadType m_loadType;
    uint64_t m_resourceUid = 0;
    sptr<ResourceFileDescription> m_fileDescription;
    sptr<ResourceGenerator> m_generator;
};

class Resource
    : public w4::core::Object
    , public w4::enable_from_this<Resource>
{
   W4_OBJECT(Resource, w4::core::Object);
public:
    using ResourceCache = core::Cache<Resource, Resource*, uint64_t>;

    Resource(const std::string& inName, const ResourceLoadDescr& descr, const std::string& subType = std::string());
    ~Resource();

    // Cache stub
    static Resource* load(uint64_t);

    template<typename T> w4::sptr<T> as() {return w4::cast_sptr<T>(shared_from_this());}

    bool isTemporary() const;
    const ResourceLoadDescr&  getDescr() const;
    void replaceDescriptor(const ResourceLoadDescr&);

    const std::string& getSubType() const;
    void setSubType(const std::string& subType);

    uint64_t getResourceUid() const;

    //toolset
    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination);

private:
    void initResourceId();
    static uint64_t generateResourceId();

private:
    ResourceLoadDescr m_descr;
    std::string m_subType;
};

} //namespace w4::resources

namespace std {
template<>
struct hash<w4::resources::ResourceGenerator>
{
    std::size_t operator()(const w4::resources::ResourceGenerator& p) const
    {
        //TODO: ^ hash of params
        return std::hash<std::string> {}(p.getType());
    }
};

template<>
struct hash<w4::resources::ResourceFileDescription>
{
    std::size_t operator()(const w4::resources::ResourceFileDescription& p) const
    {
        //TODO: ^ hash of params
        auto hasher = std::hash<std::string>();
        return hasher(p.getAssetFilePath()) ^ hasher(p.getPathInAsset());
    }
};

template<>
struct hash<w4::resources::ResourceLoadDescr>
{
    std::size_t operator()(const w4::resources::ResourceLoadDescr& p) const
    {
        switch (p.getLoadType())
        {
        case w4::resources::ResourceLoadType::File: return std::hash<uint64_t>()(p.getResourceUid());
        case w4::resources::ResourceLoadType::Generator: return std::hash<w4::resources::ResourceGenerator> {}(*p.getGenerator());
        default:break;
        }
        return 0;
    }
};
} //namespace std
