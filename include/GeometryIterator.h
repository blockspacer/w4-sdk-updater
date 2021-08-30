#pragma once

#include "IDataBuffer.h"
#include "W4Math.h"

namespace w4::resources {

class IVerticesBuffer;

struct IGeometryIter
{
protected:
    struct IterPositionData
    {
        w4::math::vec3 pos;
    };

public:
        IGeometryIter(const IGeometryIter&) = default;
    virtual operator bool() const = 0;
    virtual void operator++() = 0;
    virtual const IterPositionData* operator->() = 0;

protected:
    IGeometryIter(const resources::IDataBuffer& vBuffer, size_t vertexStructSize, size_t vertexStructPositionOffset);

    const resources::IDataBuffer& m_vBuffer;
    const size_t m_VertexStructSize;
    const size_t m_VertexStructPositionOffset;
};

struct IRawGeometryIter: public IGeometryIter
{
public:
        IRawGeometryIter(const IRawGeometryIter&) = default;

    template <typename T>
    static IRawGeometryIter createIter(const resources::IDataBuffer& vBuffer);

    template <typename T>
    const typename T::VertexFormat& get() const;

    template <typename T>
    typename T::VertexFormat& get();

    operator bool() const override;
    void operator++() override;
    const IterPositionData* operator->() override;


private:
    IRawGeometryIter(const resources::IDataBuffer& vBuffer, size_t vertexStructSize, size_t vertexStructPositionOffset);

    uint8_t* m_pos;
    const uint8_t* m_last;
};


struct IIndexedGeometryIter: public IGeometryIter
{
public:
        IIndexedGeometryIter(const IIndexedGeometryIter&) = default;

    template <typename T>
    static IIndexedGeometryIter createIter(const resources::IDataBuffer& vBuffer, const resources::IDataBuffer& iBuffer);

    template <typename T>
    const typename T::VertexFormat& get();

    operator bool() const override;
    void operator++() override;
    IIndexedGeometryIter & operator+=(int);
    const IterPositionData* operator->() override;

private:
    IIndexedGeometryIter(const resources::IDataBuffer& vBuffer, const resources::IDataBuffer& iBuffer, size_t vertexStructSize, size_t vertexStructPositionOffset);

    const resources::IDataBuffer& m_iBuffer;
    uint8_t* m_pos;
    const uint8_t* m_last;
};

#include "impl/GeometryIterator.inl"

}
