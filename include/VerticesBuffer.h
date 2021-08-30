#pragma once

#include "IVerticesBuffer.h"
#include "Cache.h"
#include "VFS.h"
#include "FatalError.h"
#include "Asset.h"

namespace w4::resources {

template<typename VertexFormat>
class VerticesBuffer
    : public IVerticesBufferSpec<VertexFormat>
{
    using Super = IVerticesBufferSpec<VertexFormat>;
public:
    static w4::sptr<VerticesBuffer<VertexFormat>> load(const ResourceLoadDescr& source);

public:
    VerticesBuffer(const ResourceLoadDescr& source, cref<filesystem::Stream> stream, uint64_t offset, uint64_t size);
    VerticesBuffer(const ResourceLoadDescr& source, std::vector<VertexFormat> && data);
    VerticesBuffer(const ResourceLoadDescr& source, const std::vector<VertexFormat>& data);

    const void* data() const override;
    uint64_t size() const override;
    BufferUsage getUsage() const override;

public:
    using IDataBuffer::outerCreator;
    using IDataBuffer::outerDeleter;

private:
    w4::sptr<filesystem::Stream> m_file;
    uint64_t m_size;
    uint64_t m_offset;
    std::vector<VertexFormat> m_data;
};

#include "impl/VerticesBuffer.inl"

} //namespace w4::resources