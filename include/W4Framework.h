#pragma once

#include "W4Common.h"
#include "W4Version.h"

// Core module

    #include "W4Logger.h"
    #include "W4Math.h"
    #include "VFS.h"
    #include "PodGenerator.h"
    #include "Event.h"
    #include "Cache.h"
    #include "IOuterID.h"
    #include "Object.h"
    #include "ProxyPtr.h"
    #include "TaskPool.h"
    #include "Timer.h"
    #include "Utils.h"
    #include "Event.h"
    #include "Track.h"
    #include "ArcBall.h"
    #include "Variant.h"
    #include "Phy.h"
    #include "FSM.h"
    #include "Rest.h"

// Platform module

    #include "Platform.h"
    #include "Input.h"
    #include "SocialMedia.h"
    #include "Facebook.h"
    #include "OK.h"
    #include "VK.h"
    #include "Audio.h"
    #include "Video.h"
    #include "Game.h"

// GUI Module

    #include "GUI.h"
    #include "Widgets/GUIButton.h"
    #include "Widgets/GUIImage.h"
    #include "Widgets/GUILabel.h"
    #include "Widgets/GUIImageButton.h"
    #include "Widgets/GUISlider.h"
    #include "Widgets/GUIComboBox.h"
    #include "Widgets/GUICheckbox.h"

// Render module

    #include "Material.h"
    #include "MaterialInstance.h"
    #include "RenderState.h"
    #include "Render.h"
    #include "RenderTarget.h"
    #include "Node.h"
    #include "Nodes/VisibleNode.h"
    #include "Nodes/DataNode.h"
    #include "Nodes/Mesh.h"
    #include "Nodes/MeshVertexDataBuilder.h"
    #include "Nodes/SkinnedMesh.h"
    #include "Nodes/Billboard.h"
    #include "Nodes/ParticlesEmitter.h"
    #include "Nodes/ArcBallNode.h"
    #include "Nodes/Camera.h"
    #include "Nodes/PointLight.h"
    #include "Nodes/Spline.h"
    #include "Nodes/SpotLight.h"
    #include "Nodes/Plotter.h"
    #include "Nodes/Root.h"
    #include "Nodes/Spine.h"
    #include "MeshVerticesBuffer.h"

    #include "Passes/NodesPass.h"
    #include "Component.h"
    #include "MovementComponent.h"
    #include "PhysicsComponent.h"
    #include "Passes/FxPass.h"

// Resources module

    #include "Resource.h"
    #include "VerticesBuffer.h"
    #include "IndicesBuffer.h"
    #include "UserIndicesBuffer.h"
    #include "UserVerticesBuffer.h"
    #include "Image.h"
    #include "Texture.h"
    #include "SimpleSkinnedAnimation.h"
    #include "Skeleton.h"
    #include "Skin.h"
    #include "Track.h"
    #include "Tween.h"
    #include "Asset.h"
    #include "Binary.h"

#define W4_USE_UNSTRICT_INTERFACE       \
    using namespace w4;                 \
    using namespace w4::core;           \
    using namespace w4::filesystem;     \
    using namespace w4::platform;       \
    using namespace w4::render;         \
    using namespace w4::resources;      \
    using namespace w4::math;           \
    using namespace w4::event;          \
    using namespace w4::gui;            \
    using namespace w4::physics;        \


#ifdef __EMSCRIPTEN__

#define W4_RUN(APP_IMPL)                                    \
    extern "C" EMSCRIPTEN_KEEPALIVE int entry(const char* name) \
    {                                                           \
        w4::Game::run<APP_IMPL>(name);                          \
        return 0;                                               \
    }                                                           \
    extern "C" EMSCRIPTEN_KEEPALIVE bool draw()                 \
    {                                                           \
        return w4::Game::getInstance()->draw();                 \
    }
#else
    #include "W4Plugins.h"
#include <thread>
#include <chrono>


#define W4_GLOBAL_SLEEP 50
#define W4_RUN(APP_IMPL)                                                            \
    int main()                                                                      \
    {                                                                               \
        w4::Game::run<APP_IMPL>("");                                                \
        auto appInst = w4::Game::getInstance();                                     \
        while(appInst->draw())                                                      \
        {                                                                           \
            std::this_thread::sleep_for(std::chrono::milliseconds(W4_GLOBAL_SLEEP));\
        }                                                                           \
        return 0;                                                                   \
    }
#endif

