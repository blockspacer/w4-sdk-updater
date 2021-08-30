#pragma once

#include "IOuterID.h"
#include "Resource.h"

namespace w4::resources {

enum class BufferType
{
    Vertices,
    Indices
};

enum class BufferUsage
{
    Static = 0,
    Dynamic = 1,
    Stream = 2
};

class IDataBuffer
    : public Resource
    , public core::IOuterManaged
{
    W4_OBJECT(IDataBuffer, Resource);
public:
    IDataBuffer(const std::string& name, const ResourceLoadDescr& descr);
    virtual ~IDataBuffer() noexcept;

    void outerCreator() override final;
    virtual void outerUpdater();
    void outerDeleter() override final;

    virtual const void* data() const = 0;
    virtual uint64_t size() const = 0;

protected:
    virtual core::OuterID::ndxType outerCreatorImpl() = 0;
    virtual BufferUsage getUsage() const = 0;
    virtual BufferType getType() const = 0;
};

} //namespace w4::resources
