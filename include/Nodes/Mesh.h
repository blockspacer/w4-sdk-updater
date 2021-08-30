#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Nodes/VisibleNode.h"
#include "W4Math.h"
#include "Resource.h"
#include "PodGenerator.h"

POD_STRUCT(MeshVertexFormat,
           POD_FIELD(w4::math::vec3, w4_a_position)
           POD_FIELD(w4::math::vec3, w4_a_normal)
           POD_FIELD(w4::math::vec3, w4_a_tangent)
           POD_FIELD(w4::math::vec3, w4_a_bitangent)
           POD_FIELD(w4::math::vec2, w4_a_uv0)
           POD_FIELD(w4::math::vec2, w4_a_uv1)
           POD_FIELD(w4::math::vec2, w4_a_uv2)
           POD_FIELD(w4::math::vec2, w4_a_uv3)
           POD_FIELD(w4::math::vec4, w4_a_color)
);

namespace w4::render {

class Mesh
        : public core::VisibleNode
{
    W4_NODE(Mesh, core::VisibleNode);

public:
    using VertexFormat = MeshVertexFormat;
public:
    Mesh(const std::string& name);
    Mesh(const std::string& name, w4::cref<resources::IVerticesBuffer> vertices);
    Mesh(NodeCloning, const Mesh&);

    void onUpdate(float dt) override;
    void onRender(const IRenderPass&) override;

    struct create
    {
        create() = delete;
       ~create() = delete;

        static w4::sptr<Mesh> box(const math::vec3& size);
        static w4::sptr<Mesh> cube(const math::vec3& size);
        static w4::sptr<Mesh> mappedCube(const math::vec3& size);
        static w4::sptr<Mesh> plane(const math::vec2& size, bool scaleUv = false);
        static w4::sptr<Mesh> plane(const math::vec2& xCoords, const math::vec2& yCoords, bool scaleUv = false);
        static w4::sptr<Mesh> sphere(float radius, uint32_t rings, uint32_t sectors);
        static w4::sptr<Mesh> cylinder(float height, float radius, uint32_t sectors);
        static w4::sptr<Mesh> cone(float radiusTop, float radiusBottom, float height, uint32_t radialSegments, uint32_t heightSegments);
        static w4::sptr<Mesh> torus(float radius, float tube, uint32_t radialSegments, uint32_t tubularSegments, float arc);
        static w4::sptr<Mesh> skybox();

        static w4::sptr<Mesh> disk(float innerRadius, float outerRadius, uint slices, float arc = w4::math::TWO_PI);

    };
};

}
