template<typename VertexFormat>
IVerticesBufferSpec<VertexFormat>::IVerticesBufferSpec(const std::string& name, const ResourceLoadDescr& descr)
    : IVerticesBuffer(name, descr)
{

}

template<typename VertexFormat>
IVerticesBufferSpec<VertexFormat>::~IVerticesBufferSpec() noexcept = default;

template<typename VertexFormat>
const core::OuterID::ndxType IVerticesBufferSpec<VertexFormat>::getVertexFormatOuterId() const
{
    return VertexFormat::getTypeId();
}

template<typename VertexFormat>
IRawGeometryIter IVerticesBufferSpec<VertexFormat>::getGeometryIterator() const
{
    return IRawGeometryIter::createIter<VertexFormat>(*this);
}

template<typename VertexFormat>
IIndexedGeometryIter IVerticesBufferSpec<VertexFormat>::getGeometryIndexedIterator(const resources::IDataBuffer& iBuffer) const
{
    return IIndexedGeometryIter::createIter<VertexFormat>(*this, iBuffer);
}