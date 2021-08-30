#pragma once

#include "Resource.h"
#include "Image.h"
#include "TextureProperties.h"
#include "Cache.h"

#include <vector>
#include <array>

namespace std {

template<>
struct hash<std::vector<w4::resources::ResourceLoadDescr>>
{
    std::size_t operator()(const std::vector<w4::resources::ResourceLoadDescr> & src) const
    {
        std::size_t res = 0;
        auto hasher = std::hash<w4::resources::ResourceLoadDescr>{};
        for(const auto &str: src)
        {
            res ^= hasher(str);
        }
        return res;
    }
};

}

namespace w4::resources {

class Cubemap: public Resource
             , public core::IOuterManaged
             , public core::Cache<Cubemap, sptr<Cubemap>, ResourceLoadDescr>
{
    W4_OBJECT(Cubemap, Resource);

    Cubemap(const ResourceLoadDescr&, std::array<sptr<Image>, 6> images);
public:
    ~Cubemap();

    static w4::sptr<Cubemap> load(const ResourceLoadDescr&);
    static w4::sptr<Cubemap> getDefault();
    static w4::sptr<Cubemap> fromImages(std::array<sptr<Image>, 6> images);
    static w4::sptr<Cubemap> fromImages(const ResourceLoadDescr &, std::array<sptr<Image>, 6> images);

    void outerCreator() override;
    void outerDeleter() override;

    Filtering getFiltering() const;
    void setFiltering(Filtering);

    Wrapping getWrappingS() const;
    void setWrappingS(Wrapping);

    Wrapping getWrappingT() const;
    void setWrappingT(Wrapping);

    void setWrappings(Wrapping wrappingS, Wrapping wrappingT);

    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination) override;

    const auto& getImages() const { return m_images; }
private:
    void update();

private:
    std::array<w4::sptr<Image>, 6> m_images;
    Filtering m_filtering = Filtering::Level0;
    Wrapping m_wrappingS = Wrapping::ClampToEdge;
    Wrapping m_wrappingT = Wrapping::ClampToEdge;
};

} //w4::resources
