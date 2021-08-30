#pragma once

#include "Resource.h"
#include "Cache.h"
#include "IOuterID.h"
#include "W4Math.h"

#include "FileStream.h"

namespace w4::resources {

class Image: public Resource
           , public core::IOuterManaged
           , public core::Cache<Image, sptr<Image>, ResourceLoadDescr>
{
    W4_OBJECT(Image, Resource);

public:
    static w4::sptr<Image> load(const ResourceLoadDescr & source);
    static w4::sptr<Image> loadFromUrl(const ResourceLoadDescr& descr, std::string url); //TODO: temporary
    static w4::sptr<Image> loadFromFile(const ResourceLoadDescr&, std::string && mimeType, cref<filesystem::Stream>, uint64_t offset, uint64_t size);
    static w4::sptr<Image> loadFromData(const ResourceLoadDescr&, std::string && mimeType, std::vector<uint8_t> && data);

public:
    ~Image();

    virtual void outerCreator() override;
    virtual void outerDeleter() override;

#if defined(W4_ENABLE_DESERIALIZERS)
    const void * getData() const;
    uint64_t getDataSize() const;
    const std::string& getMimeType() const;
#endif

private:
    Image(const ResourceLoadDescr& descr, std::string && url);
    Image(const ResourceLoadDescr& descr, std::string && mimeType, std::vector<uint8_t> && data);
    Image(const ResourceLoadDescr& descr, std::string && mimeType, cref<filesystem::Stream>, uint64_t offset, uint64_t size);

private:
    sptr<filesystem::Stream> m_file;
    uint64_t m_fileDataOffset;
    uint64_t m_fileDataSize;
    std::vector<uint8_t> m_data;
    std::string m_mimeType;

    std::string m_url;
};

} //namespace w4::resources
