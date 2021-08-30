#pragma once

#include "Resource.h"
#include "IOuterID.h"
#include "Cache.h"
#include "Image.h"
#include "TextureProperties.h"



namespace w4 {

namespace platform
{
    class Video;
}

namespace resources {

inline constexpr auto ColorTextureGenerator = "color";

inline constexpr auto TextTextureGenerator = "text";
enum class TextTextureFontStyle
{
    NORMAL_STYLE,
    ITALIC,
    OBLIQUE
};
enum class TextTextureFontWeight
{
    NORMAL_WEIGHT,
    BOLD,
    BOLDER,
    LIGHTER,
    WEIGHT_100,
    WEIGHT_200,
    WEIGHT_300,
    WEIGHT_400,
    WEIGHT_500,
    WEIGHT_600,
    WEIGHT_700,
    WEIGHT_800,
    WEIGHT_900
};

#define W4_COLOR_GENERATOR_TEXTURE(...) Texture::get(ResourceGenerator(ColorTextureGenerator, math::vec4(__VA_ARGS__)))

#define W4_PREDEFINED_COLOR_TEXTURE(NAME, ...)                                                                          \
static w4::sptr<Texture> NAME()                                                                                                      \
{                                                                                                                       \
        static auto res = W4_COLOR_GENERATOR_TEXTURE(__VA_ARGS__);                                                      \
        return res;                                                                                                     \
}

class Texture: public Resource, public core::IOuterManaged, public core::Cache<Texture, sptr<Texture>, ResourceLoadDescr>
{
    W4_OBJECT(Texture, Resource)

protected:
    Texture();
    explicit Texture(const ResourceLoadDescr& descr);

public:
    explicit Texture(const w4::sptr<Image>& image, Filtering filtering = Filtering::Level1);
    explicit Texture(const ResourceLoadDescr& descr, const w4::sptr<Image>& image);
    ~Texture();

    static void initialize();

    static w4::sptr<Texture> load(const ResourceLoadDescr & source);
    static w4::sptr<Texture> getDefault();

    cref<Image> getImage() const;

    Filtering getFiltering() const;
    void setFiltering(Filtering filtering);

    Wrapping getSWrapping() const;
    void setSWrapping(Wrapping);

    Wrapping getTWrapping() const;
    void setTWrapping(Wrapping);

    void setWrapping(Wrapping sWrapping, Wrapping tWrapping);

    const core::OuterID& getId() const;

    struct predefined
    {
        W4_PREDEFINED_COLOR_TEXTURE(white, 1.0f, 1.0f, 1.0f, 1.0f)
        W4_PREDEFINED_COLOR_TEXTURE(black, 0.0f, 0.0f, 0.0f, 1.0f)
        W4_PREDEFINED_COLOR_TEXTURE(red  , 1.0f, 0.0f, 0.0f, 1.0f)
        W4_PREDEFINED_COLOR_TEXTURE(green, 0.0f, 1.0f, 0.0f, 1.0f)
        W4_PREDEFINED_COLOR_TEXTURE(blue , 0.0f, 0.0f, 1.0f, 1.0f)
    };

    virtual void collectResourcesRecursive(std::unordered_set<sptr<resources::Resource>> & destination) override;

    const std::optional<w4::math::size>& getSize() const;
protected:
    void outerCreator() override;
    void outerDeleter() override;

    void update();

    w4::sptr<Image> m_image;

    Filtering m_filtering = Filtering::Level1;
    Wrapping m_sWrapping = Wrapping::Repeat;
    Wrapping m_tWrapping = Wrapping::Repeat;

    std::optional<w4::math::size> m_size;
};

w4::sptr<Texture> operator"" _texture(const char*, literal_int );

/*
*  ResidentTexture class is using for the setting up bitmap in memory and binding it to WebGL
*/

class ResidentTexture: public Texture
{
    W4_OBJECT(ResidentTexture, Texture)
public:
    explicit ResidentTexture(uint w, uint h);
    explicit ResidentTexture(const w4::math::size& sz);
    ~ResidentTexture();

    void setPixel(uint x, uint y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void setPixel(uint x, uint y, uint32_t value);
    uint32_t getPixel(uint x, uint y) const;

    const w4::math::size& getSize() const { return m_size; }
    uint8_t getBPP() const { return m_bpp; }

    void bind();  //TODO send data to WebGL manually

protected:
    void outerCreator() override;
    void outerDeleter() override;

    w4::math::size  m_size;
    uint8_t*        m_data = nullptr;
    uint8_t         m_bpp  = 4;

};


class VideoTexture: public Texture
{
    W4_OBJECT(VideoTexture, Texture)
    friend class w4::platform::Video;
    explicit VideoTexture(core::OuterID::ndxType videoID);
protected:
    void onUpdate();
    void outerCreator() override;
    void outerDeleter() override;

    core::OuterID::ndxType m_videoID;
};

}}
