
#pragma once

#include "Nodes/VisibleNode.h"

#include <unordered_map>

namespace w4::render {

    class RootNode final: public core::Node
    {
        W4_NODE(RootNode, core::Node);

        using DirtyFlagHandler = std::function<void ()>;

        RootNode();
        RootNode(NodeCloning, const RootNode&);
        RootNode(const std::string& name);
        ~RootNode();

        uint32_t addDirtyHandler(const DirtyFlagHandler& handler);
        void removeDirtyHandler(const uint32_t& id);

        static void preRender();

    public:
        void setDirty();
        bool isDirty();

    private:
        bool m_isDirty = true;

        static std::unordered_set<RootNode*> m_registeredRoots;
        static uint32_t m_idx;

        std::unordered_map<uint32_t, DirtyFlagHandler> m_handlers;
    };

}
