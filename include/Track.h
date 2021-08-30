#pragma once

#include <memory>
#include <unordered_set>
#include <vector>
#include <utility>
#include <tuple>

#include "W4Math.h"
#include "Object.h"
#include "Cache.h"
#include "Resource.h"
#include "Asset.h"

namespace w4::resources {

    enum class InterpType
    {
        CONST,
        LERP,
        HERMIT,
        CUBIC
    };

    template<typename T>
    struct TrackValue
    {
        float time;
        T value;
        InterpType interp;
        T tangents[2];

        bool operator<(const TrackValue &other) const
        {
            return time < other.time;
        }
    };

    template<typename T>
    class Track : public Resource
    {
        friend class Track<void>;
    public:
        using Super = Resource;
        using Value = TrackValue<T>;

        // classes / structs
        class Accessor
        {
        public:
            using sptr = w4::sptr<Accessor>;
            using wptr = w4::wptr<Accessor>;

            Accessor(const Accessor &) = delete;

            Accessor &operator=(const Accessor &) = delete;

            ~Accessor();

            void updateTime(float time);

            bool isValid() const;

            bool isInRange(float time) const;

            float getDuration() const;

            w4::sptr<Track<T>>& getTrack();

            T getValue() const;

        private:
            friend class Track<T>;

            explicit Accessor(const w4::sptr<Track> &track);

            Track<T>::Value getValueByOffset(int offset) const;
            void invalidate();

            w4::sptr<Track<T>> m_track;
            size_t m_currentValueIndex = 0;
            size_t m_nextValueIndex = 0;
            float m_deltaTime = 0.0f;
            bool m_isValid = false;
        };

    public:
        using Values = std::vector<Value>;
        typename Accessor::sptr createAccessor();

        bool isInRange(float time) const;

        float getDuration() const;

        size_t getValuesCount() const;

        size_t getAccessorsCount() const;

        void addValue(const Value &value);

        void removeValues(float fromTime, float toTime);

        void removeValues(float fromTime);

        const Values &getValues() const;

        void setValues(Values &&);

    protected:
        Track(const ResourceLoadDescr& descr);

    private:
        void removeAccessor(Accessor *accessor);

        void invalidateAccessors();

        std::unordered_set<Accessor *> m_accessors;
        Values m_values;
    };

template<typename T>
struct TrackType;

#define W4_DEFINE_TRACK(SUFFIX, TYPE) \
    struct Track##SUFFIX: public Track<TYPE>, public w4::core::Cache<Track##SUFFIX, sptr<Track##SUFFIX>, ResourceLoadDescr> \
    { \
        W4_OBJECT(Track##SUFFIX, Track<TYPE>);                                                                              \
    public:                           \
        Track##SUFFIX(const ResourceLoadDescr& descr): Super(descr) {} \
        static sptr<Track##SUFFIX> load(const ResourceLoadDescr& descr)                                                           \
        {                             \
            switch (descr.getLoadType())                                                                                    \
            {                         \
            default: W4_ASSERT(false);                          \
            case ResourceLoadType::None: return nullptr;                                                                    \
            case ResourceLoadType::Generator: FATAL_ERROR("track generators not supported"); return nullptr;                \
            case ResourceLoadType::File: return Asset::getResource<Track##SUFFIX>(*descr.getFileDescription());             \
            }\
        }\
    };                                \
    template<> struct TrackType<TYPE> { using type = Track##SUFFIX; };

    W4_DEFINE_TRACK(Float, float);
    W4_DEFINE_TRACK(Vec3, math::vec3);
    W4_DEFINE_TRACK(Rotator, math::Rotator);

#undef W4_DEFINE_TRACK

    template<typename T>
    struct ConstInterpolate
    {
        static T
        interpolate(const typename Track<T>::Value &currentValue, const typename Track<T>::Value &nextValue,
                    float delta);
    };

    template<typename T>
    struct LerpInterpolate
    {
        static T
        interpolate(const typename Track<T>::Value &currentValue, const typename Track<T>::Value &nextValue,
                    float delta);
    };

    template<>
    w4::math::Rotator LerpInterpolate<w4::math::Rotator>::interpolate(const TrackValue<w4::math::Rotator> &currentValue,
                                                                      const TrackValue<w4::math::Rotator> &nextValue,
                                                                      float delta);

    template<typename T>
    struct HermitInterpolate
    {
        static T
        interpolate(const typename Track<T>::Value &currentValue, const typename Track<T>::Value &nextValue,
                    float delta);
    };

    template<>
    w4::math::Rotator
    HermitInterpolate<w4::math::Rotator>::interpolate(const TrackValue<w4::math::Rotator> &currentValue,
                                                      const TrackValue<w4::math::Rotator> &nextValue,
                                                      float delta);

    template<typename T>
    struct CubicInterpolate
    {
        static T
        interpolate(const typename Track<T>::Value& a,
                    const typename Track<T>::Value& b,
                    const typename Track<T>::Value& c,
                    const typename Track<T>::Value& d,
                    float delta);
    };
}

#include "impl/Track.inl"

