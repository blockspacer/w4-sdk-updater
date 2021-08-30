template <typename T>
IRawGeometryIter IRawGeometryIter::createIter(const resources::IDataBuffer& vBuffer)
{
    return IRawGeometryIter(vBuffer, sizeof(T), offsetof(T, w4_a_position));
}

template <typename T>
const typename T::VertexFormat& IRawGeometryIter::get() const
{
    return reinterpret_cast<typename T::VertexFormat&>(*m_pos);
}

template <typename T>
typename T::VertexFormat& IRawGeometryIter::get()
{
    return reinterpret_cast<typename T::VertexFormat&>(*m_pos);
}

template <typename T>
IIndexedGeometryIter IIndexedGeometryIter::createIter(const resources::IDataBuffer& vBuffer, const resources::IDataBuffer& iBuffer)
{
    return IIndexedGeometryIter(vBuffer, iBuffer, sizeof(T), offsetof(T, w4_a_position));
}

template <typename T>
const typename T::VertexFormat& IIndexedGeometryIter::get()
{
    return reinterpret_cast<const typename T::VertexFormat*>(m_vBuffer.data())[*reinterpret_cast<uint32_t*>(m_pos)];
}
