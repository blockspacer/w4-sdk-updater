#pragma once

#include <string>

#include "VisibleNode.h"
#include "Mesh.h"
#include "SkinnedMesh.h"
#include "W4Math.h"
#include "PodGenerator.h"
#include "UserVerticesBuffer.h"
#include "UserIndicesBuffer.h"

POD_STRUCT(LinesVertexFormat,
           POD_FIELD(w4::math::vec3, w4_a_position)
           POD_FIELD(w4::math::vec4, w4_a_color)
);

namespace w4::render {

//Just for debug
class Plotter: public core::VisibleNode
{
W4_NODE(Plotter, core::VisibleNode);
    using VertexFormat = LinesVertexFormat;
public:
    Plotter(const std::string& name);
    Plotter(NodeCloning, const Plotter&);

    void onRender(const IRenderPass&) override;

    static w4::sptr<Plotter> build(std::vector<LinesVertexFormat> vertices, std::vector<uint32_t> indices);
    static w4::sptr<Plotter> buildFromSegments(std::vector<std::array<w4::math::vec3, 2>> segments, const w4::math::vec4& color);
    static w4::sptr<Plotter> buildFromPolyline(std::vector<w4::math::vec3> points, const w4::math::vec4& color);
    static w4::sptr<Plotter> buildSphere(float radius, size_t rings, size_t sectors, const w4::math::vec4& color);
    static w4::sptr<Plotter> buildOctahedron(std::array<w4::math::vec3, 8> vertices, const w4::math::vec4& color);
    static w4::sptr<Plotter> buildCube(std::array<w4::math::vec3, 2> inVertices, const w4::math::vec4& color);
    static w4::sptr<Plotter> buildCapsule(float radius, float height, size_t rings, size_t sectors, const w4::math::vec4& color);
    static w4::sptr<Plotter> buildMesh(w4::cref<Mesh>, w4::math::vec4::cref color);
    static w4::sptr<Plotter> buildMesh(w4::cref<SkinnedMesh>, w4::math::vec4::cref color);
    static w4::sptr<Plotter> buildRay(math::vec3::cref startPoint, math::vec3::cref directionPoint, const w4::math::vec4& color);
    static w4::sptr<Plotter> buildLine(math::vec3::cref from, math::vec3::cref to, const w4::math::vec4& color);
    static w4::sptr<Plotter> buildUnitsSnap(float unitSize, float size);

    void setLines(std::vector<LinesVertexFormat> vertices, std::vector<uint32_t> indices);


    static w4::sptr<Node> addAxisViev(w4::sptr<Node>, float size = 50.0f );


private:
    void init();
    static size_t m_plotterCounter;
};

class PlotterVerticesBuffer: public resources::UserVerticesBuffer<Plotter::VertexFormat>
{
    W4_OBJECT(PlotterVerticesBuffer, resources::UserVerticesBuffer<Plotter::VertexFormat>);

public:
    explicit PlotterVerticesBuffer();

    resources::BufferUsage getUsage() const override;
};

class PlotterIndicesBuffer: public resources::UserIndicesBuffer
{
    W4_OBJECT(PlotterIndicesBuffer, resources::UserIndicesBuffer);
public:
    explicit PlotterIndicesBuffer(const std::string& name);

    resources::BufferUsage getUsage() const override;
};

}
