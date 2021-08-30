#pragma once

#include "Node.h"
#include "VerticesBuffer.h"
#include "GeometryIterator.h"
#include "Event.h"
#include "Input.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "IIndicesBuffer.h"

namespace w4::core {

struct StringHash
{
    constexpr StringHash(const char * string): name(string), hash(utils::hash(string)) {}
    StringHash(uint32_t) = delete;

    const char* name;
    uint32_t hash;
};

class VisibleNode;
class Surface: public Object, public IOuterManaged
{
    W4_OBJECT(Surface, Object);
protected:
    struct SurfaceCloning{};

public:
    Surface(const Surface&) = delete;
    Surface(Surface&&) = delete;
    Surface& operator=(const Surface&) = delete;
    Surface& operator=(Surface&&) = delete;

    Surface(VisibleNode&, const std::string&);
    Surface(SurfaceCloning, const Surface&, VisibleNode&);
    virtual ~Surface();

    virtual uptr<Surface> clone(VisibleNode & owner) const;

    virtual void outerCreator() override;
    virtual void outerDeleter() override;

    virtual void onRender(const render::IRenderPass & pass);

    Surface& setMaterialInst(cref<resources::MaterialInst>);
    virtual Surface& setIndicesBuffer(cref<resources::IIndicesBuffer>);

    VisibleNode & getOwner();
    const VisibleNode & getOwner() const;
    cref<resources::MaterialInst> getMaterialInst() const;
    cref<resources::IIndicesBuffer> getIndicesBuffer() const;

    resources::IIndexedGeometryIter getGeometryIndexedIterator() const;

    void setShadowCaster(bool flag);
    virtual void setShadowReceiver(bool flag);

    bool isShadowCaster() const;
    bool isShadowReceiver() const;

    uint32_t getRenderType() const;

    virtual void onVerticesBufferChanged(cref<resources::IVerticesBuffer>);

protected:
    friend class VisibleNode;

    virtual void onMaterialChanged(cref<resources::MaterialInst>);
    template<typename T>
    void checkResource(cref<T> resource);

protected:
    VisibleNode & m_owner;
    sptr<resources::MaterialInst> m_materialInst;

    bool m_shadowCaster = true;
    bool m_shadowReceiver = true;

private:
    sptr<resources::IIndicesBuffer> m_indicesBuffer;
};

class VisibleNode : public Node
{
    W4_NODE(VisibleNode, Node);
public:
    VisibleNode(const std::string& name);
    VisibleNode(NodeCloning, const VisibleNode&);
    VisibleNode(const std::string& name, w4::cref<resources::IVerticesBuffer> vertices);
    virtual ~VisibleNode();

    w4::cref<resources::IVerticesBuffer> getVerticesBuffer() const;
    void setVerticesBuffer(w4::cref<resources::IVerticesBuffer>);

    int32_t getRenderOrder() const;
    void setRenderOrder(int32_t renderOrder);

    bool isInFrustum(const render::Camera& camera) const override;

    Surface& addSurface(const std::string& name);
    Surface& addSurface(const std::string& name, cref<resources::IIndicesBuffer> indices);
    Surface& addSurface(const std::string& name, cref<resources::IIndicesBuffer> indices, cref<resources::MaterialInst> materialInstance);

    Surface& getFirstSurface();
    Surface& getSurface(StringHash);

    void removeSurface(const std::string& name);

    void foreachSurface(std::function<void(Surface&)>);
    void foreachSurface(std::function<void(const Surface&)>) const;

    uint32_t getSurfacesCount() const;

    void setMaterialInst(cref<resources::MaterialInst> m);
    cref<resources::MaterialInst> getMaterialInst() const;
    cref<resources::MaterialInst> getMaterialInst(StringHash surfaceName) const;

    bool isShadowCaster() const;
    bool isShadowReceiver() const;
    bool isIgnoreInShadowsAABB()const;

    void setShadowCaster(bool flag);
    void setShadowReceiver(bool flag);
    void setIgnoreInShadowsAABB(bool flag);

    virtual void onRender(const render::IRenderPass&) override;

    resources::IRawGeometryIter getGeometryIterator() const;
    size_t getGeometryPointsCount() const;

    void updateCommonMaterial();

    virtual uint32_t getRenderType() const;

//tool
    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination) override;

protected:
    virtual void onSurfaceAdded(const Surface& surface);
    virtual void onSurfaceRemoved(const Surface& surface);

protected:
    std::unordered_map<uint32_t, uptr<Surface>> m_surfaces;

    void updateFrustumCollider();

private:
    friend class Surface;
    void onSurfaceMaterialChanged(w4::cref<resources::MaterialInst> materialInst);

private:
    int32_t m_renderOrder = 0;
    sptr<resources::MaterialInst> m_commonMaterialInst;
    w4::sptr<resources::IVerticesBuffer> m_verticesBuffer;

    bool m_ignoreInShadowsAABB = false;
};

} //namespaces

namespace w4::render {
//TODO: move visible node to w4::render?
using w4::core::Surface;
}

template<typename T>
void w4::core::Surface::checkResource(cref<T> resource)
{
    if(resource == nullptr)
    {
        FATAL_ERROR("[Node '%s'][Surface '%s] unable to set null %s", m_owner.getName().data(), getName().data(), T::typeInfo.name());
    }
    else if (!resource->isValid())
    {
        FATAL_ERROR("[Node '%s'][Surface '%s] unable to set invalid %s", m_owner.getName().data(), getName().data(), T::typeInfo.name());
    }
}