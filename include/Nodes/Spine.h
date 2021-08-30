
#pragma once

#include "Nodes/VisibleNode.h"

#include <unordered_map>

#include "UserVerticesBuffer.h"
#include "UserIndicesBuffer.h"
#include "spine/spine.h"

POD_STRUCT(SpineVertexFormat,
           POD_FIELD(w4::math::vec3, w4_a_position)
           POD_FIELD(w4::math::vec2, w4_a_uv0)
           POD_FIELD(w4::math::vec4, w4_a_color)
);

namespace w4::resources
{
    class SpineVerticesBuffer
        : public IVerticesBufferSpec<SpineVertexFormat>
    {
        W4_OBJECT(SpineVerticesBuffer, IVerticesBufferSpec<SpineVertexFormat>);
    public:
        using IVerticesBufferSpec<SpineVertexFormat>::IVerticesBufferSpec;

        SpineVerticesBuffer(const ResourceLoadDescr& source)
            :IVerticesBufferSpec<SpineVertexFormat>(source.getName(), source)
        {
            m_data = new SpineVertexFormat[5000];
            m_capacity = 5000;
        }

        ~SpineVerticesBuffer()
        {
            delete[] m_data;
        }

        resources::BufferUsage getUsage() const override
        {
            return resources::BufferUsage::Dynamic;
        }

        SpineVertexFormat& getNextItem()
        {
            if (m_size >= m_capacity)
            {
                SpineVertexFormat* newData = new SpineVertexFormat[m_capacity*2];
                memcpy(newData, m_data, m_capacity * sizeof(SpineVertexFormat));
                delete[] m_data;
                m_data = newData;
                m_capacity *= 2;
            }

            auto idx = m_size;
            ++m_size;
            return m_data[idx];
        }

        const void* data() const override
        {
            return m_data;
        }

        uint64_t size() const override
        {
            return m_size * sizeof(SpineVertexFormat);
        }

        SpineVertexFormat* m_data = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
    };

    class SpineIndicesBuffer
            : public IIndicesBuffer
    {
        W4_OBJECT(SpineIndicesBuffer, IIndicesBuffer);
    public:
        using IIndicesBuffer::IIndicesBuffer;

        SpineIndicesBuffer(const ResourceLoadDescr& source)
            :IIndicesBuffer(source.getName(),source)
        {
            m_data = new IndexType[5000];
            m_capacity = 5000;
        }

        ~SpineIndicesBuffer()
        {
            delete[] m_data;
        }

        BufferUsage getUsage() const override
        {
            return resources::BufferUsage::Dynamic;
        }

        IndexType& getNextItem()
        {
            if (m_size >= m_capacity)
            {
                IndexType* newData = new IndexType[m_capacity*2];
                memcpy(newData, m_data, m_capacity * sizeof(IndexType));
                delete[] m_data;
                m_data = newData;
                m_capacity *= 2;
            }

            auto idx = m_size;
            ++m_size;
            return m_data[idx];
        }

        const void* data() const override
        {
            return m_data;
        }

        uint64_t size() const override
        {
            return m_size * sizeof(IndexType);
        }

        IndexType* m_data = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
    };

    struct SpineAtlas : public core::Cache<SpineAtlas, sptr<SpineAtlas>, std::string>
    {
        w4::sptr<spine::Atlas> atlas;

        static w4::sptr<SpineAtlas> load(const std::string& file);
    };

    struct SpineSkeletonData : public core::Cache<SpineSkeletonData, sptr<SpineSkeletonData>, std::pair<std::string, std::string>>
    {
        w4::sptr<spine::SkeletonData> data;

        static w4::sptr<SpineSkeletonData> load(const std::pair<std::string, std::string>& file);
    };
}
namespace w4::render {

    class Spine final: public core::VisibleNode
    {
        W4_NODE(Spine, core::VisibleNode);

    public:
        using VertexFormat = SpineVertexFormat;

        //Spine(const std::string& name, const w4::sptr<spine::SkeletonData>& skeletonData, const w4::sptr<spine::AnimationStateData>& stateData = nullptr);
        Spine(const std::string& name, const std::string& atlasFile, const std::string& skeletonFile, float scaleFactor = 1024.f, const w4::sptr<spine::AnimationStateData>& stateData = nullptr);
        Spine(NodeCloning cloning, const Spine& from);
        ~Spine();

        void onUpdate(float dt) override;
        void onRender(const IRenderPass& pass) override;

        void setUsePremultipliedAlpha(bool usePMA) { usePremultipliedAlpha = usePMA; };

        bool getUsePremultipliedAlpha() { return usePremultipliedAlpha; };

        void setDepthTestFlag(bool flag) { useDepthTest = flag; }

        w4::uptr<spine::AnimationState> state = nullptr;
        float timeScale = 1.0f;
        w4::uptr<spine::Skeleton> skeleton = nullptr;

    private:

        struct SpineSurface
        {
            w4::sptr<w4::resources::MaterialInst> m_spineMaterial;
            w4::sptr<Surface> m_drawSurface;
            w4::sptr<resources::SpineIndicesBuffer> m_indicesBuffer;
        };

        SpineSurface createNewSpineSurface();

        bool usePremultipliedAlpha = false;
        bool useDepthTest = true;
        spine::VertexEffect* vertexEffect = nullptr;

        spine::Vector<float> worldVertices;
        spine::Vector<unsigned short> quadIndices;
        spine::SkeletonClipping clipper;

        w4::sptr<spine::AnimationStateData> m_stateData;


        w4::sptr<resources::SpineVerticesBuffer> m_verticesUserBuffer;

        std::vector<SpineSurface> m_drawSurfaces;
        size_t m_drawSurfacesCount = 0;

        float m_scaleFactor;
    };

    class SpineTextureLoader : public spine::TextureLoader {
    public:
        virtual void load(spine::AtlasPage &page, const spine::String &path);

        virtual void unload(void *texture);

        spine::String toString() const;

    private:
        std::unordered_set<w4::sptr<w4::resources::Texture>*> m_loadedTextures;
    };

}
