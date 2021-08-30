#pragma once

#include "Utils.h"
#include "Cache.h"
#include "W4Common.h"
#include "IOuterID.h"

namespace w4::resources {

enum class ShaderType
{
    NONE,
    VERTEX,
    FRAGMENT
};

class Shader: public core::IOuterManaged, public core::Cache<Shader, sptr<Shader>, std::pair<utils::CRCWrapper, ShaderType>>
{
public:
    static sptr<Shader> load(const std::pair<utils::CRCWrapper, ShaderType>& uri);

public:
    void outerCreator() override;
    void outerDeleter() override;

private:
    explicit Shader(const std::string& text, ShaderType type);

private:
    ShaderType m_type = ShaderType::NONE;
    std::string m_text;
};

class ShaderProgram : public core::Cache<ShaderProgram, sptr<ShaderProgram>, std::pair<sptr<Shader>, sptr<Shader>>>
{
public:
    static sptr<ShaderProgram> load(const std::pair<sptr<Shader>, sptr<Shader>>&);

    cref<Shader> getVertex() const;
    cref<Shader> getFragment() const;

private:
    ShaderProgram(cref<Shader> vertex, cref<Shader> fragment);

private:
    sptr<Shader> m_vertex;
    sptr<Shader> m_fragment;
};

}