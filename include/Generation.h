#pragma once

#include "Texture.h"
#include "Passes/OffscreenPass.h"

namespace w4::resources {


// TODO extract in a common part module / header

class TextureOp : public w4::core::Object
{
W4_OBJECT(TextureOp, w4::core::Object)

public:
    virtual bool isValid() const = 0;
    virtual bool perform() = 0;

    virtual w4::sptr<Texture> getResult() const = 0;

    ~TextureOp() {}
};

class TextureGenerator : public w4::core::Object
{
W4_OBJECT(TextureGenerator, w4::core::Object)

public:
    void push(w4::sptr<TextureOp> op);
    w4::sptr<Texture> getResult() const;
    virtual bool perform();

protected:
    std::vector<w4::sptr<TextureOp>> m_pipeline;
};

class GenerateWhiteNoise : public TextureOp
{
W4_OBJECT(GenerateWhiteNoise, TextureOp)

public:
    GenerateWhiteNoise(uint w, uint h);

    bool isValid() const override;
    bool perform() override;

    w4::sptr<Texture> getResult() const override;
private:
    w4::sptr<ResidentTexture> m_result;
};

class PassTextureOp: public TextureOp
{
public:
    PassTextureOp(uint w, uint h);

    bool isValid() const override;
    bool perform() override;
    w4::sptr<Texture> getResult() const override;
protected:
    w4::render::OffscreenPass& getPass() { return m_pass; }
private:
    w4::render::OffscreenPass m_pass;
};


class GenerationRect: public PassTextureOp
{
W4_OBJECT(GenerationRect, PassTextureOp)
public:
    GenerationRect(uint w, uint h);
};


class PerlinNoise: public PassTextureOp
{
    W4_OBJECT(PerlinNoise, PassTextureOp)
public:
    PerlinNoise(uint w, uint h);

    bool isValid() const override;

    void setFrequency(float frequency);
    void setOctaves(float octaves);
    void setLacunarity(float lacunarity);
    void setGain(float gain);
    void setSeed(float seed);
private:
    w4::sptr<resources::MaterialInst> m_MatInst;
};

class BlendOp: public PassTextureOp
{
    W4_OBJECT(BlendOp, PassTextureOp)
public:
    BlendOp(uint w, uint h, w4::sptr<Texture> in1, w4::sptr<Texture> in2);
private:
    w4::sptr<resources::MaterialInst> matInst;
};

}