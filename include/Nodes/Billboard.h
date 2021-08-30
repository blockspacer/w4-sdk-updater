#pragma once

#include "Nodes/Mesh.h"

namespace w4::render {

class Billboard final: public core::VisibleNode
{
    W4_NODE(Billboard, core::VisibleNode);

public:
    Billboard(const std::string & name);
    Billboard(NodeCloning, const Billboard&);
    explicit Billboard(const w4::math::vec2& size, const std::string & name = defaultName);
    Billboard(float width, float height, const std::string & name = defaultName);

    const w4::math::vec2& getSize() const;
    void setSize(const w4::math::vec2&);
    void setSize(float width, float height);

    void onRender(const IRenderPass&) override;

private:
    void init();
    void updateGeometry();

private:
    w4::math::vec2 m_size;
};

}
