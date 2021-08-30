#pragma once

#include "W4Common.h"
#include "W4Logger.h"
#include "TypeInfo.h"

namespace w4::core {

#define W4_OBJECT_IMPL(PREFIX, T, S)                                          \
public:                                                                       \
    W4_TYPEINFO(T, PREFIX, S)                                                 \

#define W4_OBJECT(T, S) W4_OBJECT_IMPL("Object", T, S)

struct CopyableObject
{
    explicit CopyableObject(const std::string& name = defaultName);
    virtual ~CopyableObject() = default;

    [[nodiscard]] const std::string& getName()  const;
    void setName(const std::string& name);

    [[nodiscard]] virtual const TypeInfo& getTypeInfo() const = 0;

// public API

    virtual void log(uint indent=0) const;
    virtual void onNameChange()     {}
    virtual void onUpdate(float dt) {};

    static constexpr const char* const defaultName = "unnamed";
    static constexpr TypeInfo typeInfo{"Object"};
private:

    std::string  m_name;

};

struct Object : CopyableObject
{
    using CopyableObject::CopyableObject;

    Object(Object const&) = delete;
    Object(Object &&) = delete;
    Object &operator=(const Object&) = delete;
    Object &operator=(Object &&) = delete;
};

} //namespace
