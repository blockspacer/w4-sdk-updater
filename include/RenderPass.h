#pragma once

#include <map>

#include "Nodes/Mesh.h"
#include "Nodes/Camera.h"

#include "RenderTarget.h"

namespace w4::render {

class IRenderPass
{
public:

    IRenderPass(bool enabled);

    virtual ~IRenderPass() {}

    virtual void begin() = 0;
    virtual void draw() = 0;
    virtual void end() = 0;
    virtual cref<Camera> getCamera() const;

    bool isEnabled() const;
    void setEnabled(bool value);

protected:
    virtual void onEnabled() {}

    bool m_enabled;
};

}