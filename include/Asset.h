#pragma once

#include <functional>

#include "Object.h"
#include "Resource.h"
#include "Node.h"
#include "VFS.h"

namespace w4::render {
class ParticlesEmitter;
}

namespace w4::resources {

class Asset: public w4::core::Object, public w4::core::Cache<Asset>
{
    W4_OBJECT(Asset, w4::core::Object);
public:
    static sptr<Asset> load(const std::string& path);
    bool save(const std::string& path) const;

public:
    template<typename T>
    static sptr<T> getResource(const ResourceFileDescription& description);

public:
    Asset(const std::string& name);

    static const std::string& getDefaultAssetPath();
    static sptr<Asset> getDefault();

    bool isEmpty() const;

    void addResource(cref<Resource>);
    void addParticle(std::string path, cref<render::ParticlesEmitter>);
    void removeResourcesIf(std::function<bool(cref<Resource>)>);

    template<typename T>
    bool removeResource(cref<T>);
    template<typename T>
    std::vector<sptr<T>> getAllResources() const;

    template<typename T>
    bool hasResource(std::string path) const;

    template<typename T>
    sptr<T> getResource(std::string path) const;
    template<typename T>
    sptr<T> getResource(const char* path) const;
    sptr<render::ParticlesEmitter> getParticle(std::string path) const;

    void addRoot(const std::string& name, cref<core::Node>);
    sptr<core::Node> getFirstRoot() const;
    sptr<core::Node> getRoot(const std::string& name) const;

    cref<core::Node> getFirstRootMutable();
    cref<core::Node> getRootMutable(const std::string& name);

    const std::unordered_map<std::string, sptr<core::Node>>& getAllRootsMutable();
    void removeAllRoots();

private:
    static std::string& normalizePath(std::string & path);
    sptr<Resource> getResource(std::string && path, const core::TypeInfo& ti) const;

private:
    std::unordered_set<sptr<Resource>> m_resources;
    std::unordered_map<core::TypeInfo::HashType, std::unordered_map<std::string, sptr<Resource>>> m_resourcesByPath;
    std::unordered_map<std::string, sptr<core::Node>> m_roots;
    std::unordered_map<std::string, sptr<render::ParticlesEmitter>> m_particles;
};

template<typename T>
sptr<T> Asset::getResource(std::string path) const
{
    static_assert(core::is_base_of_v<Resource, T>);
    return std::static_pointer_cast<T>(getResource(std::move(path), T::typeInfo));
}

template<typename T>
sptr<T> Asset::getResource(const char* path) const
{
    return getResource<T>(std::string(path));
}

template<typename T>
bool Asset::removeResource(cref<T> resource)
{
    if (m_resources.erase(resource))
    {
        auto typeIt = m_resourcesByPath.find(T::typeInfo.hash());
        typeIt->second.erase(resource->getDescr().getFileDescription()->getPathInAsset());
        if (typeIt->second.empty())
        {
            m_resourcesByPath.erase(typeIt);
        }
        return true;
    }
    return false;
}

template<typename T>
std::vector<sptr<T>> Asset::getAllResources() const
{
    std::vector<sptr<T>> result;
    if (auto it = m_resourcesByPath.find(T::typeInfo.hash()); it != m_resourcesByPath.end())
    {
        for (auto &[path, resource]: it->second)
        {
            result.emplace_back(resource->template as<T>());
        }
    }
    return result;
}

template<typename T>
bool Asset::hasResource(std::string path) const
{
    normalizePath(path);
    if (auto it = m_resourcesByPath.find(T::typeInfo.hash()); it != m_resourcesByPath.end())
    {
        return it->second.count(path) > 0;
    }
    return false;
}

template<typename T>
sptr<T> Asset::getResource(const ResourceFileDescription& description)
{
    if (auto asset = Asset::get(description.getAssetFilePath()))
    {
        if (auto resource = asset->getResource<T>(description.getPathInAsset()))
        {
            return resource;
        }
        else
        {
            FATAL_ERROR("'%s' '%s' is not found in asset '%s'",
                         T::typeInfo.hash(),
                         description.getPathInAsset().c_str(),
                         description.getAssetFilePath().c_str());
        }
    }
    else
    {
        FATAL_ERROR("asset '%s' is not found", description.getAssetFilePath().c_str());
    }
    static sptr<T> empty;
    return empty;
}

template<>
sptr<render::ParticlesEmitter> Asset::getResource(std::string path) const;

} //namespace w4::resources
