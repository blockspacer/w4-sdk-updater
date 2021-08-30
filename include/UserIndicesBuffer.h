#pragma once

#include "IIndicesBuffer.h"
#include "Cache.h"
#include "FileStream.h"

namespace w4::resources {

class UserIndicesBuffer
    : public IIndicesBuffer
    , public core::Cache<UserIndicesBuffer, sptr<UserIndicesBuffer>, ResourceLoadDescr>
{
    W4_OBJECT(UserIndicesBuffer, IIndicesBuffer);
public:
    UserIndicesBuffer(const ResourceLoadDescr& descr);
    UserIndicesBuffer(const ResourceLoadDescr& source, cref<filesystem::Stream> stream, uint64_t offset, uint64_t size);

    static w4::sptr<UserIndicesBuffer> load(const ResourceLoadDescr& source);

    UserIndicesBuffer& clean();
    UserIndicesBuffer &append(IndexType);
    UserIndicesBuffer &append(std::initializer_list<IndexType>);
    UserIndicesBuffer &append(const IndexType *begin, size_t size);

    const void* data() const override;
    uint64_t size() const override;
    BufferUsage getUsage() const override;
    std::vector<IndexType>::const_iterator cbegin() const;
    std::vector<IndexType>::const_iterator cend() const;
private:
    std::vector<IndexType> m_indices;
};

} //namespace w4::resources
