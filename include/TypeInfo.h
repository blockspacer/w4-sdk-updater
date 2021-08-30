#pragma once

#include "Utils.h"
#include "FatalError.h"

#include <unordered_map>

namespace w4::core {

#define W4_TYPEINFO(TYPE, PREFIX, PARENTCLASS)                                               \
    using Super = PARENTCLASS;                                                               \
    static constexpr w4::core::TypeInfo typeInfo{PREFIX "_" #TYPE, Super::typeInfo};         \
    virtual const w4::core::TypeInfo& getTypeInfo() const override                           \
    {                                                                                        \
         return typeInfo;                                                                    \
    }                                                                                        \
    template<typename RefType>                                                               \
    bool is() const                                                                          \
    {                                                                                        \
        return w4::core::ti_is_same_v(getTypeInfo(), RefType::typeInfo);                     \
    }                                                                                        \
    template<typename RefType>                                                               \
    bool derived_from() const                                                                \
    {                                                                                        \
        return w4::core::ti_is_base_of_v(RefType::typeInfo, getTypeInfo());                  \
    }


    struct TypeInfo
{
    using HashType =  uint32_t;
    constexpr TypeInfo(const char* name) : m_name(name), m_hash(w4::utils::hash(name)), m_parent(nullptr) {}
    constexpr TypeInfo(const char* name, const TypeInfo &parent) : m_name(name), m_hash(w4::utils::hash(name)), m_parent(&parent) {}
    [[nodiscard]] constexpr HashType hash() const {return m_hash;}
    [[nodiscard]] constexpr const char* name() const {return m_name;}
    [[nodiscard]] constexpr const TypeInfo * parentInfo() const {return m_parent;}
    [[nodiscard]] constexpr bool operator==(const TypeInfo& rh) const { return m_hash == rh.m_hash; }
    [[nodiscard]] constexpr bool operator!=(const TypeInfo& rh) const { return !operator==(rh); }

private:
    const char* m_name;
    HashType m_hash;
    const TypeInfo * m_parent;
};

// Need for serialization
struct TypeInfoHash
{
    std::string m_name;
};

static constexpr bool ti_is_base_of_v(const TypeInfo& B, const TypeInfo& D)
{
    auto p = &D;
    while(p)
    {
        if(*p == B)
        {
            return true;
        }
        p = p->parentInfo();
    }
    return false;
}
template<typename B, typename D>
static constexpr bool is_base_of_v = ti_is_base_of_v(B::typeInfo, D::typeInfo);

static constexpr bool ti_is_same_v(const TypeInfo& B, const TypeInfo& D)
{
    return B == D;
};

template<typename B, typename D>
static constexpr bool is_same_v = ti_is_same_v(B::typeInfo, D::typeInfo);

template<bool B, class T = void> struct enable_if {};
template<typename T> struct enable_if<true, T> { using type = T; };
template<bool B, typename T = void> using enable_if_t = typename enable_if<B, T>::type;

} //namespace w4::core
