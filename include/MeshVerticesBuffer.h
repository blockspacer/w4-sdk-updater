#pragma once

#include "VerticesBuffer.h"
#include "Nodes/Mesh.h"
#include "Nodes/SkinnedMesh.h"
#include "UserVerticesBuffer.h"

namespace w4::resources
{
    class MeshVerticesBuffer
            : public VerticesBuffer<render::Mesh::VertexFormat>
            , public core::Cache<MeshVerticesBuffer, sptr<MeshVerticesBuffer>, ResourceLoadDescr>
    {
        W4_OBJECT(MeshVerticesBuffer, VerticesBuffer<render::Mesh::VertexFormat>);
    public:
        using VerticesBuffer<render::Mesh::VertexFormat>::VerticesBuffer;
        static sptr<MeshVerticesBuffer> load(const ResourceLoadDescr& source)
        {
            if (source.getLoadType() != ResourceLoadType::File)
            {
                FATAL_ERROR("generators are not supported for MeshVerticesBuffer");
            }
            return Asset::getResource<MeshVerticesBuffer>(*source.getFileDescription());
        }
    };

    class SkinnedMeshVerticesBuffer
            : public VerticesBuffer<render::SkinnedMesh::VertexFormat>
            , public core::Cache<SkinnedMeshVerticesBuffer, sptr<SkinnedMeshVerticesBuffer>, ResourceLoadDescr>
    {
        W4_OBJECT(SkinnedMeshVerticesBuffer, VerticesBuffer<render::Mesh::VertexFormat>);
    public:
        using VerticesBuffer<render::SkinnedMesh::VertexFormat>::VerticesBuffer;
        static sptr<SkinnedMeshVerticesBuffer> load(const ResourceLoadDescr& source)
        {
            if (source.getLoadType() != ResourceLoadType::File)
            {
                FATAL_ERROR("generators are not supported for SkinnedMeshVerticesBuffer");
            }
            return Asset::getResource<SkinnedMeshVerticesBuffer>(*source.getFileDescription());
        }
    };

#define SPECIALIZE_USER_VERTEX_BUFFER(PREFIX, VERTEX_FORMAT)                                                                        \
class PREFIX##UserVerticesBuffer: public UserVerticesBuffer<VERTEX_FORMAT>, public core::Cache<PREFIX##UserVerticesBuffer>          \
{                                                                                                                                   \
    W4_OBJECT(PREFIX##UserVerticesBuffer, UserVerticesBuffer<VERTEX_FORMAT>);                                                       \
public:                                                                                                                             \
    explicit PREFIX##UserVerticesBuffer(const ResourceLoadDescr& source): Super(source) {}                                          \
    PREFIX##UserVerticesBuffer(const ResourceLoadDescr& source, cref<filesystem::Stream> stream, uint64_t offset, uint64_t size)    \
        : Super(source, stream, offset, size) {}                                                                                    \
    static w4::sptr<PREFIX##UserVerticesBuffer> load(const ResourceLoadDescr& source)                                               \
    {                                                                                                                               \
        if (source.getLoadType() != ResourceLoadType::File)                                                                         \
        {                                                                                                                           \
            FATAL_ERROR("generators are not supported for " #PREFIX "UserVerticesBuffer");                                          \
        }                                                                                                                           \
        if (source.getFileDescription()->getAssetFilePath().empty())                                                                \
        {                                                                                                                           \
            return make::sptr<PREFIX##UserVerticesBuffer>(source);                                                                  \
        }                                                                                                                           \
        return Asset::getResource<PREFIX##UserVerticesBuffer>(*source.getFileDescription());                                        \
    }                                                                                                                               \
}                                                                                                                                   \

SPECIALIZE_USER_VERTEX_BUFFER(Mesh, MeshVertexFormat);
SPECIALIZE_USER_VERTEX_BUFFER(SkinnedMesh, SkinnedMeshVertexFormat);

#undef SPECIALIZE_USER_VERTEX_BUFFER

}