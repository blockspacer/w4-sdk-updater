#pragma once

#include "Resource.h"
#include "FileStream.h"

namespace w4::resources {

class Binary: public Resource, public w4::core::Cache<Binary, w4::sptr<Binary>, ResourceLoadDescr>
{
    W4_OBJECT(Binary, Resource);
public:
    static w4::sptr<Binary> load(const ResourceLoadDescr & source);
    static w4::sptr<Binary> loadFromFile(const ResourceLoadDescr&, cref<filesystem::Stream>, uint64_t offset, uint64_t size);
    static w4::sptr<Binary> loadFromData(const ResourceLoadDescr&, std::vector<uint8_t> && data);

public:
    virtual ~Binary();

    const uint8_t * data() const;
    uint64_t size() const;

private:
    Binary(const ResourceLoadDescr& descr, std::vector<uint8_t> && data);
    Binary(const ResourceLoadDescr& descr, cref<filesystem::Stream>, uint64_t offset, uint64_t size);

private:
    sptr<filesystem::Stream> m_file;
    uint64_t m_fileDataOffset;
    uint64_t m_fileDataSize;
    std::vector<uint8_t> m_data;
};

} //namespace w4::resources
