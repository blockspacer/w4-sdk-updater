#pragma once

#include "IDataBuffer.h"

namespace w4::resources {

class IIndicesBuffer
    : public IDataBuffer
{
    W4_OBJECT(IIndicesBuffer, IDataBuffer);
public:
    using IndexType = uint32_t;

public:
    explicit IIndicesBuffer(const std::string name, const ResourceLoadDescr& descr);
    virtual ~IIndicesBuffer() noexcept;

    BufferType getType() const override final;

    void outerUpdater() override final;
    void outerUpdaterIndicesUsage();

    virtual size_t getIndicesOffset() const;
    virtual size_t getIndicesCount() const;

private:
    core::OuterID::ndxType outerCreatorImpl() override;
};

} //namespace w4::resources