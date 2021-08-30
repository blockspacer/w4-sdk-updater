#pragma once

#include "Node.h"

namespace w4::core {

enum class LightDecayRate : uint32_t
{
    None,
    Linear,
    Quadratic,
    Cubic
};

class DataNode : public Node
{
    W4_NODE(DataNode, Node);

    explicit DataNode(const std::string& src);
    DataNode(NodeCloning, const DataNode&);
    virtual ~DataNode() noexcept = default;
};

} //namespaces
