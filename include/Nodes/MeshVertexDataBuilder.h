#pragma once

#include <utility>

#include "UserVerticesBuffer.h"
#include "UserIndicesBuffer.h"
#include "Nodes/Mesh.h"

namespace w4::resources {

class MeshVertexDataBuilder
{
public:
    using VertexVector = std::vector<render::Mesh::VertexFormat>;
    using IndexVector = std::vector<uint32_t>;

    using VerticesBufferType = resources::UserVerticesBuffer<render::Mesh::VertexFormat>;
    using IndicesBufferType = resources::UserIndicesBuffer;

    explicit MeshVertexDataBuilder(w4::cref<VerticesBufferType> vertices, w4::cref<IndicesBufferType> indices)
        : m_verticesBuffer(vertices)
        , m_indicesBuffer(indices)
    {
    }

    void addVertex(float x, float y, float z, float u, float v);
    void addVertex(math::vec3::cref , math::vec2::cref, math::vec3::cref);
    void addVertex(math::vec3::cref , math::vec2::cref, math::vec3::cref, math::vec3::cref);

    void addIndices(const IndexVector& indices);
    void build();

    void buildVertexBuffer();
    void buildIndexBuffer();

    size_t getVerticesCount() const;
    size_t getIndicesCount() const;

    void setIndexBuffer(w4::cref<IndicesBufferType> indices);
    void calculate();
private:
    math::vec3 calcNormal(const math::vec3& ab, const math::vec3& ac);

    VertexVector m_vertices;
    IndexVector m_indices;
    w4::sptr<VerticesBufferType> m_verticesBuffer;
    w4::sptr<IndicesBufferType> m_indicesBuffer;
};

}