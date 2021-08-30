template<typename VertexFormat>
UserVerticesBuffer<VertexFormat>::UserVerticesBuffer(const ResourceLoadDescr& source)
    : IVerticesBufferSpec<VertexFormat>(source.getName(), source)
{

}

template<typename VertexFormat>
UserVerticesBuffer<VertexFormat>::UserVerticesBuffer(const ResourceLoadDescr& source, cref<filesystem::Stream> stream, uint64_t offset, uint64_t size)
    : UserVerticesBuffer(source)
{
    if (size % sizeof(VertexFormat) > 0)
    {
        FATAL_ERROR("UserVerticesBuffer: invalid vertices buffer input file");
    }
    append((const VertexFormat*)(stream->data() + offset), size / sizeof(VertexFormat));
    Super::outerCreator();
}

template<typename VertexFormat>
UserVerticesBuffer<VertexFormat>& UserVerticesBuffer<VertexFormat>::clear()
{
    m_vertices.clear();
    return *this;
}

template<typename VertexFormat>
UserVerticesBuffer<VertexFormat>& UserVerticesBuffer<VertexFormat>::append(const VertexFormat& vertex)
{
    m_vertices.emplace_back(vertex);
    return *this;
}

template<typename VertexFormat>
UserVerticesBuffer<VertexFormat>& UserVerticesBuffer<VertexFormat>::append(std::initializer_list<VertexFormat> vertices)
{
    m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
    return *this;
}

template<typename VertexFormat>
UserVerticesBuffer<VertexFormat>& UserVerticesBuffer<VertexFormat>::append(const VertexFormat *begin, size_t size)
{
    m_vertices.insert(m_vertices.end(), begin, begin + size);
    return *this;
}

template<typename VertexFormat>
VertexFormat &UserVerticesBuffer<VertexFormat>::operator[](size_t index)
{
    W4_ASSERT( index < m_vertices.size());
    return m_vertices[index];
}

template<typename VertexFormat>
const void* UserVerticesBuffer<VertexFormat>::data() const
{
    return m_vertices.data();
}

template<typename VertexFormat>
uint64_t UserVerticesBuffer<VertexFormat>::size() const
{
    return m_vertices.size() * sizeof(VertexFormat);
}

template<typename VertexFormat>
size_t UserVerticesBuffer<VertexFormat>::verticesCount() const
{
    return m_vertices.size();
}

template<typename VertexFormat>
void UserVerticesBuffer<VertexFormat>::removeVertex(size_t index)
{
    W4_ASSERT( index < m_vertices.size());
    m_vertices.erase(m_vertices.begin() + index);
}

template<typename VertexFormat>
BufferUsage UserVerticesBuffer<VertexFormat>::getUsage() const
{
    return BufferUsage::Static;
}