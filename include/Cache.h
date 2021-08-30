#pragma once

#include <unordered_map>
#include <vector>
#include <functional>

#include "W4Common.h"
#include "FatalError.h"

namespace w4::core {

template<typename T, typename PtrType>
struct RefCreator
{
    static const PtrType& create(const PtrType& ptr);
};

template<typename T, typename PtrType>
struct CopyCreator
{
    static PtrType create(const PtrType& ptr);
};

template<typename T, typename PtrType>
struct CloneCreator
{
    static PtrType create(const PtrType& ptr);
};

template<typename T,
         typename PtrType = sptr<T>,
         typename KeyType = std::string,
         typename CreatorType = RefCreator<T, PtrType>>
class Cache
{
public:
    struct GCStrategy_NotUsed;
    struct GCStrategy_All;

    static void add(const KeyType & key, PtrType obj);
    [[maybe_unused]] static PtrType getOptional(KeyType);

    template<typename Creator = CreatorType>
    static PtrType get(const KeyType& key);

    [[maybe_unused]] static void foreachObject(const std::function<void(const PtrType&)>& func);
    static void remove(const KeyType& key);
    static bool contains(const KeyType& key);
    [[maybe_unused]] static void resetCache();

    template<class GCStrategy = GCStrategy_NotUsed>
    static void collectGarbage(uint8_t shrinkFactor = 0);

    [[maybe_unused]] static size_t cacheSize();

private:
    struct CacheObj
    {
        KeyType key;
        PtrType ptr;

        CacheObj() = default;
        CacheObj(const KeyType &k, PtrType p): key(k), ptr(std::move(p)){}
    };

    static void shrink();

    static uint8_t m_foreachDepth;
    static bool m_needShrink;

    static std::unordered_map<KeyType, size_t> m_indexMap;
    static std::vector<CacheObj> m_values;
};

#include "impl/Cache.inl"

}
