
template<typename T, typename PtrType>
const PtrType& RefCreator<T, PtrType>::create(const PtrType& ptr)
{
    return ptr;
}

template<typename T, typename PtrType>
PtrType CopyCreator<T, PtrType>::create(const PtrType& ptr)
{
    return PtrType(new T(*ptr));
}

template<typename T, typename PtrType>
PtrType CloneCreator<T, PtrType>::create(const PtrType& ptr)
{
    return ptr->clone()->template as<T>();
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
struct Cache<T, PtrType, KeyType, CreatorType>::GCStrategy_NotUsed
{
    static bool isValid(PtrType const& ptr)
    {
        return ptr.use_count() > 1;
    }
};

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
struct Cache<T, PtrType, KeyType, CreatorType>::GCStrategy_All
{
    static bool isValid(PtrType const& ptr)
    {
        return false;
    }
};

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
void Cache<T, PtrType, KeyType, CreatorType>::add(const KeyType & key, PtrType obj)
{
    auto it = m_indexMap.find(key);
    if(it != m_indexMap.end())
    {
        W4_ASSERT( false);
        return;
    }
    m_values.emplace_back(key, std::move(obj));
    m_indexMap.emplace(key, m_values.size() - 1);
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
PtrType Cache<T, PtrType, KeyType, CreatorType>::getOptional(const KeyType key)
{
    auto it = m_indexMap.find(key);
    return it == m_indexMap.end() ? nullptr : m_values[it->second].ptr;
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
template<typename Creator>
PtrType Cache<T, PtrType, KeyType, CreatorType>::get(const KeyType& key)
{
    auto it = m_indexMap.find(key);
    if (it == m_indexMap.end())
    {
        m_values.push_back(CacheObj{key, T::load(key)});
        it = m_indexMap.emplace(key, static_cast<int>(m_values.size()) - 1).first;
    } else{
        if (!m_values[it->second].ptr)
        {
            m_values[it->second].ptr = T::load(key);
        }
    }

    return Creator::create(m_values[it->second].ptr);
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
void Cache<T, PtrType, KeyType, CreatorType>::foreachObject(const std::function<void(const PtrType&)>& func)
{
    m_foreachDepth++;
    int idx = 0;
    while (idx < m_values.size())
    {
        auto obj = m_values[idx];
        if (obj.ptr)
        {
            func(obj.ptr);
        }
        ++idx;
    }

    m_foreachDepth--;

    if (m_needShrink)
    {
        shrink();
    }
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
void Cache<T, PtrType, KeyType, CreatorType>::remove(const KeyType& key)
{
    auto it = m_indexMap.find(key);
    if(it != m_indexMap.end())
    {
        m_values[it->second].ptr = nullptr;
        m_indexMap.erase(it);
    }
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
template<class Strategy>
void Cache<T, PtrType, KeyType, CreatorType>::collectGarbage(uint8_t shrinkFactor)
{
    auto nItemsToRemove = 0;
    for(auto& v : m_values)
    {
        if(!Strategy::isValid(v.ptr))
        {
            v.ptr = nullptr;
        }

        if(!v.ptr)
        {
            nItemsToRemove++;
        }
    }

    if (nItemsToRemove > shrinkFactor)
    {
        shrink();
    }
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
void Cache<T, PtrType, KeyType, CreatorType>::shrink()
{
    if (m_foreachDepth > 0)
    {
        m_needShrink = true;
        return;
    }

    auto values = std::move(m_values);
    auto indexes = std::move(m_indexMap);

    for (auto& v : values)
    {
        if(v.ptr)
        {
            m_values.push_back(v);
            m_indexMap[v.key] = m_values.size() - 1;
        }
    }
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
bool Cache<T, PtrType, KeyType, CreatorType>::contains(const KeyType& key)
{
    return m_indexMap.find(key) != m_indexMap.end();
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
void Cache<T, PtrType, KeyType, CreatorType>::resetCache()
{
    m_values.clear();
    m_indexMap.clear();
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
size_t Cache<T, PtrType, KeyType, CreatorType>::cacheSize()
{
    return m_indexMap.size();
}

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
std::unordered_map<KeyType, size_t> Cache<T, PtrType, KeyType, CreatorType>::m_indexMap;

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
std::vector<typename Cache<T, PtrType, KeyType, CreatorType>::CacheObj> Cache<T, PtrType, KeyType, CreatorType>::m_values;

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
uint8_t Cache<T, PtrType, KeyType, CreatorType>::m_foreachDepth = 0;

template<typename T, typename PtrType, typename KeyType, typename CreatorType>
bool Cache<T, PtrType, KeyType, CreatorType>::m_needShrink = false;
