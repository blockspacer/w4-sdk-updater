#pragma once

//#include <sstream>
//#include <iomanip>

#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <array>
#include <functional>
#include "W4Math.h"

#include "IOuterID.h"
#include "W4Logger.h"

namespace w4::multiplayer {

class SubscribableCallbacksStorage
{
    static size_t m_onChangedCallbacksCounter;
    static std::map<size_t, std::function<void()>> m_onChangedCallbacks;

    static std::set<size_t> m_postponedCallbacks;
public:
     static size_t registerCallback(std::function<void()>);
     static void unregisterCallback(size_t idx);
     static void postponeCallback(size_t idx);
     static void callPostponedCallbacks();
};

class SubscribableChangesCollector
{
    std::map<size_t, float const *> m_floats;
    std::map<size_t, int32_t const *> m_ints;
    std::map<size_t, bool const *> m_bools;
    std::map<size_t, std::string const *> m_strings;
public:
    size_t flushData2Buf(uint8_t* buf, size_t bufLen);
    void changeValue(size_t idx, const float& value);
    void changeValue(size_t idx, const int32_t& value);
    void changeValue(size_t idx, const bool& value);
    void changeValue(size_t idx, const std::string& value);
    void changeValue(size_t idx, const w4::math::vec2& value);
    void changeValue(size_t idx, const w4::math::vec3& value);
    void changeValue(size_t idx, const w4::math::vec4& value);
};

template<class OwnerClass>
class Subscribable
{
    size_t m_onChanged = 0;
protected:
    OwnerClass& m_owner;
    std::unique_ptr<SubscribableChangesCollector> m_changesCollectorStorage;
    SubscribableChangesCollector& m_changesCollector;
public:
    Subscribable(OwnerClass& owner): m_owner(owner), m_changesCollector(owner.getChangesCollector()) {}

    SubscribableChangesCollector& getChangesCollector()
    {
        return m_owner.getChangesCollector();
    }

    void onChanged() const
    {
        if(m_onChanged)
        {
            SubscribableCallbacksStorage::postponeCallback(m_onChanged);
        }
        m_owner.onChanged();
    }

    void subscribe(const std::function<void()>& onChangedHandler)
    {
        m_onChanged = SubscribableCallbacksStorage::registerCallback(onChangedHandler);
    }

    void unsubscribe()
    {
        SubscribableCallbacksStorage::unregisterCallback(m_onChanged);
        m_onChanged = 0;
    }
};

template<>
Subscribable<std::nullptr_t>::Subscribable(std::nullptr_t& owner);

template<>
void Subscribable<std::nullptr_t>::onChanged() const;

template<>
SubscribableChangesCollector& Subscribable<std::nullptr_t>::getChangesCollector();

extern std::nullptr_t nothing;

template<class D, class S>
D jsonCast(const S& src)
{
    return static_cast<D>(src);
}

template<>
std::string jsonCast(const int& src);

template<>
std::string jsonCast(const float& src);

template<>
std::string jsonCast(const bool& src);

template<>
float jsonCast(const std::string& src);

template<>
int jsonCast(const std::string& src);

template<>
bool jsonCast(const std::string& src);

template<>
w4::math::vec2 jsonCast(const bool& src);

template<>
w4::math::vec3 jsonCast(const bool& src);

template<>
w4::math::vec4 jsonCast(const bool& src);

template<>
w4::math::vec2 jsonCast(const std::string& src);

template<>
w4::math::vec3 jsonCast(const std::string& src);

template<>
w4::math::vec4 jsonCast(const std::string& src);

class HandlerStorage
{
    std::map<size_t, std::function<void(size_t, const bool&)>> m_boolHandlers;
    std::map<size_t, std::function<void(size_t, const float_t&)>> m_floatHandlers;
    std::map<size_t, std::function<void(size_t, const int32_t&)>> m_intHandlers;
    std::map<size_t, std::function<void(size_t, const std::string&)>> m_stringHandlers;
    std::map<size_t, std::function<void(size_t, const w4::math::vec2&)>> m_vec2Handlers;
    std::map<size_t, std::function<void(size_t, const w4::math::vec3&)>> m_vec3Handlers;
    std::map<size_t, std::function<void(size_t, const w4::math::vec4&)>> m_vec4Handlers;

    std::map<size_t, bool> m_boolDiff;
    std::map<size_t, float_t> m_floatDiff;
    std::map<size_t, int32_t> m_intDiff;
    std::map<size_t, std::string> m_stringDiff;
    std::map<size_t, const w4::math::vec2> m_vec2Diff;
    std::map<size_t, const w4::math::vec3> m_vec3Diff;
    std::map<size_t, const w4::math::vec4> m_vec4Diff;

    template<typename T>
    std::map<size_t, std::function<void(size_t, const T&)>>& getStorage();

    template<typename T, typename T2>
    bool callHandlerInternal(size_t idx, T2 val)
    {
        auto iter = getStorage<T>().find(idx);
        if(iter == getStorage<T>().end())
        {
            return false;
        }
        iter->second(idx, jsonCast<T, T2>(val));
        return true;
    }
public:

    template<typename T>
    void setHandler(size_t idx, size_t idxCount, std::function<void(size_t, const T&)> handler)
    {
        for(auto i = idx; i < idx + idxCount; ++i)
        {
            getStorage<T>()[i] = handler;
        }
    }

    template<typename T>
    bool callHandler(size_t idx, T val)
    {
        if(callHandlerInternal<float_t, T>(idx, val))
            return true;
        if(callHandlerInternal<int32_t, T>(idx, val))
            return true;
        if(callHandlerInternal<w4::math::vec2, T>(idx, val))
            return true;
        if(callHandlerInternal<w4::math::vec3, T>(idx, val))
            return true;
        if(callHandlerInternal<w4::math::vec4, T>(idx, val))
            return true;
        if(callHandlerInternal<std::string, T>(idx, val))
            return true;
        if(callHandlerInternal<bool, T>(idx, val))
            return true;
        return false;
    }
};

template<>
std::map<size_t, std::function<void(size_t, const bool&)>>& HandlerStorage::getStorage();

template<>
std::map<size_t, std::function<void(size_t, const float_t&)>>& HandlerStorage::getStorage();

template<>
std::map<size_t, std::function<void(size_t, const int32_t&)>>& HandlerStorage::getStorage();

template<>
std::map<size_t, std::function<void(size_t, const std::string&)>>& HandlerStorage::getStorage();

template<>
std::map<size_t, std::function<void(size_t, const w4::math::vec2&)>>& HandlerStorage::getStorage();

template<>
std::map<size_t, std::function<void(size_t, const w4::math::vec3&)>>& HandlerStorage::getStorage();

template<>
std::map<size_t, std::function<void(size_t, const w4::math::vec4&)>>& HandlerStorage::getStorage();

enum class TypeEnum: uint32_t {
    String = 0,
    Float = 1,
    Int = 2,
    Bool = 3
};

template<class DerivedClass, class OwnerClass>
class Dictionary: public Subscribable<OwnerClass>
{
//    size_t m_indicesCounter = 0;
//    HandlerStorage m_storage;
public:
    Dictionary(OwnerClass& owner): Subscribable<OwnerClass>(owner)
    {
    }

    template<typename T>
    size_t captureIndexes(size_t indicesCount, std::function<void(size_t, const T&)> setup)
    {
        return Subscribable<OwnerClass>::m_owner.captureIndexes(indicesCount, setup);
    }
/*
    void parseArray(uint8_t* data, size_t len)
    {
        size_t pos = 0;
        while(pos < len)
        {
            size_t valType = *reinterpret_cast<uint32_t*>(data + pos);
            const auto key = valType & 0xffff;
            valType >>= 16;
            pos += 4;
//            W4_LOG_ERROR("Changed key: %d type %d", key, valType);
            switch(static_cast<TypeEnum>(valType))
            {
                case TypeEnum::Bool:
                    m_storage.callHandler(key, *reinterpret_cast<int32_t*>(data + pos) != 0);
                    pos += 4;
                    break;
                case TypeEnum::Int:
                    m_storage.callHandler(key, *reinterpret_cast<int32_t*>(data + pos));
                    pos += 4;
                    break;
                case TypeEnum::Float:
//                    W4_LOG_ERROR("Changed key: %d = %f", key, *reinterpret_cast<float_t*>(data + pos));
                    m_storage.callHandler(key, *reinterpret_cast<float_t*>(data + pos));
                    pos += 4;
                    break;
                case TypeEnum::String:
                    uint16_t strLen = *reinterpret_cast<uint16_t*>(data + pos);
                    pos += 2;
                    m_storage.callHandler(key, std::string(reinterpret_cast<char*>(data + pos), strLen));
                    const auto alignment = (strLen + 2) % 4;
                    pos += strLen + (alignment ? (4 - alignment): 0);
                    break;
            }
        }
        SubscribableCallbacksStorage::callPostponedCallbacks();
    }

    size_t buildArray(uint8_t* data, size_t maxLen)
    {
        return Subscribable<OwnerClass>::getChangesCollector().flushData2Buf(data, maxLen);
    }
*/
    template<class OwnerClass2>
    Dictionary& operator=(const Dictionary<DerivedClass, OwnerClass2>& src)
    {
        *this = reinterpret_cast<const Dictionary<DerivedClass, OwnerClass>&>(src);
    }
};

template<class DerivedClass>
class Dictionary<DerivedClass, std::nullptr_t>: public Subscribable<std::nullptr_t>
{
    size_t m_indicesCounter = 0;
    HandlerStorage m_storage;
public:
    Dictionary(): Subscribable<std::nullptr_t>(nothing)
    {
    }

    void parseArray(uint8_t* data, size_t len)
    {
/*
        for(size_t i = 0; i < len;)
        {
            size_t j = i;
            std::stringstream dump;
            for(; j < std::min(len, i + 100); ++j)
            {
                dump << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(data[j]) << "|";
            }
            dump << std::endl;
            W4_LOG_ERROR("%d-%d: %s", i, j, dump.str().c_str());
            i = j;
        }
*/
        size_t pos = 0;
        while(pos < len)
        {
            size_t valType = *reinterpret_cast<uint32_t*>(data + pos);
            const auto key = valType & 0xffff;
            valType >>= 16;
            pos += 4;
//            W4_LOG_ERROR("Changed key: %d type %d", key, valType);
            switch(static_cast<TypeEnum>(valType))
            {
                case TypeEnum::Bool:
                    m_storage.callHandler(key, *reinterpret_cast<int32_t*>(data + pos) != 0);
                    pos += 4;
                    break;
                case TypeEnum::Int:
                    m_storage.callHandler(key, *reinterpret_cast<int32_t*>(data + pos));
                    pos += 4;
                    break;
                case TypeEnum::Float:
//                    W4_LOG_ERROR("Changed key: %d = %f", key, *reinterpret_cast<float_t*>(data + pos));
                    m_storage.callHandler(key, *reinterpret_cast<float_t*>(data + pos));
                    pos += 4;
                    break;
                case TypeEnum::String:
                    uint16_t strLen = *reinterpret_cast<uint16_t*>(data + pos);
                    pos += 2;
                    m_storage.callHandler(key, std::string(reinterpret_cast<char*>(data + pos), strLen));
                    const auto alignment = (strLen + 2) % 4;
                    pos += strLen + (alignment ? (4 - alignment): 0);
                    break;
            }
        }
        SubscribableCallbacksStorage::callPostponedCallbacks();
    }

    size_t buildArray(uint8_t* data, size_t maxLen)
    {
        return Subscribable<std::nullptr_t>::getChangesCollector().flushData2Buf(data, maxLen);
    }

    template<class OwnerClass2>
    Dictionary& operator=(const Dictionary<DerivedClass, OwnerClass2>& src)
    {
        *this = reinterpret_cast<const Dictionary<DerivedClass, std::nullptr_t>&>(src);
    }

    template<typename T>
    size_t captureIndexes(size_t indicesCount,
                          std::function<void(size_t, const T &)> setup)
    {
        const auto retVal = m_indicesCounter;
        m_indicesCounter += indicesCount;
        m_storage.setHandler<T>(retVal, indicesCount, setup);
        return retVal;
    }
};

namespace get
{
    template<typename T> constexpr size_t IndicesCount()       { return 1;}
    template<> constexpr size_t IndicesCount<w4::math::vec2>() { return 2;}
    template<> constexpr size_t IndicesCount<w4::math::vec3>() { return 3;}
    template<> constexpr size_t IndicesCount<w4::math::vec4>() { return 4;}
}




template <class E, size_t N, class OwnerClass = std::nullptr_t>
class Array: public Subscribable<OwnerClass>
{
public:
    typedef E value_type;

    Array(): Array(nothing, std::make_index_sequence<N>{}) {}

    Array(OwnerClass& owner) : Array(owner, std::make_index_sequence<N>{}) {}

    template<typename T>
    size_t captureIndexes(size_t indicesCount, std::function<void(size_t, const T&)> setup)
    {
        return Subscribable<OwnerClass>::m_owner.captureIndexes(indicesCount, setup);
    }

    E& operator[](size_t idx)
    {
        return m_data[idx];
    }

    const E& operator[](size_t idx) const
    {
        return m_data[idx];
    }

    size_t size() const
    {
        return N;
    }

    template<class Owner2>
    Array& operator=(const Array<E, N, Owner2>& src)
    {
        size_t idx = 0;
        for(auto& val: m_data)
        {
            val = src.m_data[idx++];
        }
    }

private:
    template<size_t... Is>
    Array(OwnerClass& owner, std::index_sequence<Is...>) : Subscribable<OwnerClass>(owner),
            m_data{((void)Is, E{*this})...}
    {}

    std::array<E, N> m_data;
};

template<class T>
T passNewValue(const T& oldValue, const T& newValue)
{
    return newValue;
}

template<class T, class OwnerClass = std::nullptr_t> class Property: public Subscribable<OwnerClass>
{
    T m_value;
    size_t m_firstIndex;
    std::function<T(const T&, const T&)> m_preprocessValue = passNewValue<T>;

    template <class U = T>
    typename std::enable_if<std::is_same<U, w4::math::vec2>::value ||
                            std::is_same<U, w4::math::vec3>::value ||
                            std::is_same<U, w4::math::vec4>::value, void>::type
    outerSetImpl( size_t idx, const U& val )
    {
        const auto relativeIdx = idx - m_firstIndex;
        if(m_value[relativeIdx] != val[relativeIdx])
        {
            m_value[relativeIdx] = val[relativeIdx];
            Subscribable<OwnerClass>::onChanged();
        }
    }

    template <class U = T>
    typename std::enable_if<!(std::is_same<U, w4::math::vec2>::value ||
                              std::is_same<U, w4::math::vec3>::value ||
                              std::is_same<U, w4::math::vec4>::value), void>::type
    outerSetImpl( size_t idx, const U& val )
    {
        if(m_value != val)
        {
            m_value = m_preprocessValue(m_value, val);
            Subscribable<OwnerClass>::onChanged();
        }
    }

    template<class Owner2>
    Property& operator=(const Property<T, Owner2>& src)
    {
        m_value = src.m_value;
    }

public:
    template<typename X> constexpr size_t getIndicesCount() {return get::IndicesCount<X>();}

    Property(): Subscribable<OwnerClass>(nullptr)
    {}

    Property(const T& defaultValue): Subscribable<OwnerClass>(nullptr), m_value(defaultValue)
    {}

    template <class U = T>
    Property(const T& defaultValue,
             std::function<T(const T&, const T&)> valuePreprocessor,
             typename std::enable_if<!(std::is_same<U, w4::math::vec2>::value ||
                                       std::is_same<U, w4::math::vec3>::value ||
                                       std::is_same<U, w4::math::vec4>::value), void>::type): Property(defaultValue)
    {
        setValuePreprocessor(valuePreprocessor);
    }

    Property(OwnerClass& owner): Subscribable<OwnerClass>(owner)
    {
        std::function<void(size_t, const T&)> func = std::bind(&Property<T, OwnerClass>::outerSet, this, std::placeholders::_1, std::placeholders::_2);
        m_firstIndex = owner.captureIndexes(getIndicesCount<T>(), func);
    }

    Property(OwnerClass& owner, const T& defaultValue): Subscribable<OwnerClass>(owner), m_value(defaultValue)
    {
        std::function<void(size_t, const T&)> func = std::bind(&Property<T, OwnerClass>::outerSet, this, std::placeholders::_1, std::placeholders::_2);
        m_firstIndex = owner.captureIndexes(getIndicesCount<T>(), func);
    }

    template <class U = T>
    Property(OwnerClass& owner,
             const T& defaultValue,
             std::function<T(const T&, const T&)> valuePreprocessor,
             typename std::enable_if<!(std::is_same<U, w4::math::vec2>::value ||
                                       std::is_same<U, w4::math::vec3>::value ||
                                       std::is_same<U, w4::math::vec4>::value), void>::type* = 0): Property(owner, defaultValue)
    {
        setValuePreprocessor(valuePreprocessor);
    }

    void operator = ( const T &val )
    {
        set( val );
    }

    void set( const T &val )
    {
        if(m_value != val)
        {
            m_value = val;
            Subscribable<OwnerClass>::m_changesCollector.changeValue(m_firstIndex, m_value);
        }
    }

    void outerSet( size_t idx, const T& val )
    {
        outerSetImpl(idx, val);
    }

    operator const T& () const
    {
        return get();
    }

    const T& get() const
    {
        return m_value;
    }

    void setValuePreprocessor(std::function<T(const T&, const T&)> valuePreprocessor)
    {
        m_preprocessValue = valuePreprocessor;
    }
};

enum class State
{
    LoggingIn,
    LoggingError,
    WebsockConnecting,
    WebsockError,
    MatchmakingRequested,
    MatchmakingError,
    MatchMakingWaiting,
    Complete
};

class MultiplayerHandler
{
    w4::multiplayer::Dictionary<std::nullptr_t, std::nullptr_t>& m_data;
    std::function<void(size_t)> m_playerIdxCallback;
    std::function<void(size_t)> m_playersCountCallback;
    std::function<void(size_t)> m_playerLeaveCallback;
    std::function<void(w4::multiplayer::State, size_t playersCount)> m_stateCallback;
    std::array<uint8_t, 100 * 1024> m_array;
public:
    struct Stat
    {
        size_t idx = 0;
        float min = NAN;
        float max = NAN;
        float avg = NAN;
        float stddev = NAN;
        float delay = NAN;
    };

    MultiplayerHandler(w4::multiplayer::Dictionary<std::nullptr_t, std::nullptr_t>& data, std::function<void(size_t)> playerIdxCallback, std::function<void(size_t)> playersCountCallback, std::function<void(size_t)> playerLeaveCallback, std::function<void(w4::multiplayer::State, size_t)> stateCallback);
    ~MultiplayerHandler();
    void updateData();
    void disconnect();
    void rematch();

    inline w4::multiplayer::Dictionary<std::nullptr_t, std::nullptr_t>& getDataStruct() { return m_data; }
    inline void callPlayerIdxCallback(size_t idx) {m_playerIdxCallback(idx);}
    inline void callPlayersCountCallback(size_t count) {m_playersCountCallback(count);}
    inline void callPlayerLeaveCallback(size_t idx) {m_playerLeaveCallback(idx);}
    inline void callStateCallback(w4::multiplayer::State state, size_t playersCount) {m_stateCallback(state, playersCount);}

    inline void setOuterId(int32_t id) {m_outerId.set(id);}

    const Stat& getStat();
    void onUpdateReceived();

    uint8_t* getDataPtr();
    size_t getDataLen() const;

    inline bool isOnline() const {return m_isOnline;}
    inline void setOnlineState(bool online) {m_isOnline = online;}

    void setUpdateCallback(std::function<void()> updateCallback);
    void resetUpdateCallback();

private:
    bool m_isOnline = true;

    static constexpr size_t m_statWindowsSize = 50;
    float m_statDiffs[m_statWindowsSize];
    size_t m_curStatCount = 0;
    Stat m_stat;
    bool m_statIsStarted = false;
    clock_t m_prevClock;

    w4::core::OuterID m_outerId;

    std::function<void()> m_updateCallback;
};

sptr<MultiplayerHandler> connect(const std::string& w4BackendUri, const std::string& w4MatchmackingUri, const std::string& w4WebsockUri, int32_t userLevel, int32_t campaignSize, const std::string& manifestId, w4::multiplayer::Dictionary<std::nullptr_t, std::nullptr_t>& data, std::function<void(size_t)> playerIdxCallback, std::function<void(size_t)> playersCountCallback, std::function<void(size_t)> playerLeaveCallbackk, std::function<void(w4::multiplayer::State, size_t)> stateCallback);

} //w4::multiplayer
