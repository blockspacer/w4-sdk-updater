#pragma once

#include "IIndicesBuffer.h"
#include "VFS.h"
#include "Cache.h"


namespace w4::resources {

class IndicesBuffer final
    : public IIndicesBuffer
    , public core::Cache<IndicesBuffer, sptr<IndicesBuffer>, ResourceLoadDescr>
{
    W4_OBJECT(IndicesBuffer, IIndicesBuffer);
public:
    static w4::sptr<IndicesBuffer> load(const ResourceLoadDescr& file);

public:
    IndicesBuffer(const ResourceLoadDescr& descr, cref<filesystem::Stream> stream, uint64_t offset, uint64_t size);
    IndicesBuffer(const ResourceLoadDescr& descr, std::vector<IndexType> && data);
    IndicesBuffer(const ResourceLoadDescr& descr, const std::vector<IndexType>& data);
    virtual ~IndicesBuffer() noexcept;

    const void* data() const override;
    uint64_t size() const override;
    BufferUsage getUsage() const override final;

private:
    w4::sptr<filesystem::Stream> m_file;
    uint64_t m_fileOffset;
    uint64_t m_fileSize;
    std::vector<IndexType> m_data;
};

} //namespace w4::resources