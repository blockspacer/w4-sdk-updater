#pragma once

#include "IVerticesBuffer.h"
#include "Cache.h"
#include "FileStream.h"

namespace w4::resources {

template<typename VertexFormat>
class UserVerticesBuffer
    : public IVerticesBufferSpec<VertexFormat>
{
public:
    using Super = IVerticesBufferSpec<VertexFormat>;

public:
    static w4::sptr<UserVerticesBuffer> load(const ResourceLoadDescr& source);

public:
    virtual ~UserVerticesBuffer() = default;

    UserVerticesBuffer& clear();

    UserVerticesBuffer& append(const VertexFormat&);
    UserVerticesBuffer& append(std::initializer_list<VertexFormat>);
    UserVerticesBuffer& append(const VertexFormat *begin, size_t size);

    VertexFormat &operator[](size_t index);
    size_t verticesCount() const;

    void removeVertex(size_t index);

    const void* data() const override;
    uint64_t size() const override;
    BufferUsage getUsage() const override;

protected:
    explicit UserVerticesBuffer(const ResourceLoadDescr& source);
    explicit UserVerticesBuffer(const ResourceLoadDescr& source, cref<filesystem::Stream>, uint64_t offset, uint64_t size);

private:
    std::vector<VertexFormat> m_vertices;
};

#include "impl/UserVerticesBuffer.inl"

} //namespace w4::resources