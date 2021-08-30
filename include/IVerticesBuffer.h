#pragma once

#include "IDataBuffer.h"
#include "GeometryIterator.h"

namespace w4::resources {

class IVerticesBuffer
    : public IDataBuffer
{
    W4_OBJECT(IVerticesBuffer, IDataBuffer);
public:
    IVerticesBuffer(const std::string& name, const ResourceLoadDescr& descr);
    virtual ~IVerticesBuffer() noexcept;

    virtual IRawGeometryIter getGeometryIterator() const = 0;
    virtual IIndexedGeometryIter getGeometryIndexedIterator(const resources::IDataBuffer& iBuffer) const = 0;
    virtual const core::OuterID::ndxType getVertexFormatOuterId() const = 0;

protected:
    virtual core::OuterID::ndxType outerCreatorImpl() override;

private:
    BufferType getType() const override final;
};

template<typename VertexFormat>
class IVerticesBufferSpec
    : public IVerticesBuffer
{
    //W4_OBJECT(IVerticesBufferSpec<VertexFormat>, IVerticesBuffer);
    using Super = IVerticesBuffer;
public:
    using Format = VertexFormat;

public:
    IVerticesBufferSpec(const std::string& name, const ResourceLoadDescr& descr);
    virtual ~IVerticesBufferSpec() noexcept;

    IRawGeometryIter getGeometryIterator() const override;
    IIndexedGeometryIter getGeometryIndexedIterator(const resources::IDataBuffer& iBuffer) const override;
private:
    const core::OuterID::ndxType getVertexFormatOuterId() const override;
};

#include "impl/IVerticesBuffer.inl"

} //namespace w4::resources
