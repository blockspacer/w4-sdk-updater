template<typename VertexFormat>
VerticesBuffer<VertexFormat>::VerticesBuffer(const ResourceLoadDescr& descr, cref<filesystem::Stream> stream, uint64_t offset, uint64_t size)
    : IVerticesBufferSpec<VertexFormat>(descr.getName(), descr)
{
    if  (descr.getLoadType() != ResourceLoadType::File)
    {
        FATAL_ERROR("VerticesBuffer: not file load type");
    }

    if (stream == nullptr)
    {
        FATAL_ERROR("null stream");
    }
    if (size % sizeof(VertexFormat) != 0)
    {
        FATAL_ERROR("invalid vertices file '%s'", stream->getPath().c_str());
    }

#if defined(EMSCRIPTEN)
    m_file = stream;
    m_offset = offset;
    m_size = size;
#else
    m_data.assign((const VertexFormat*)stream->data() + offset, (const VertexFormat*)stream->data() + offset + size / sizeof(VertexFormat));
#endif
    outerCreator();
}

template<typename VertexFormat>
VerticesBuffer<VertexFormat>::VerticesBuffer(const ResourceLoadDescr& descr, std::vector<VertexFormat> && data)
    : IVerticesBufferSpec<VertexFormat>(descr.getName(), descr)
    , m_data(std::move(data))
{
    outerCreator();
}

template<typename VertexFormat>
VerticesBuffer<VertexFormat>::VerticesBuffer(const ResourceLoadDescr& descr, const std::vector<VertexFormat>& data)
    : IVerticesBufferSpec<VertexFormat>(descr.getName(), descr)
    , m_data(data)
{
    outerCreator();
}

template<typename VertexFormat>
const void* VerticesBuffer<VertexFormat>::data() const
{
    return m_file ? m_file->data() + m_offset : (const void*)m_data.data();
}

template<typename VertexFormat>
uint64_t VerticesBuffer<VertexFormat>::size() const
{
    return m_file ? m_size : m_data.size() * sizeof(VertexFormat);
}

template<typename VertexFormat>
BufferUsage VerticesBuffer<VertexFormat>::getUsage() const
{
    return BufferUsage::Static;
}