#include "FatalError.h"
#include "VFS.h"

namespace w4::resources {

//template<typename T>
//Track<T>::Track(const ResourceLoadDescr& descr)
//    : w4::resources::Resource(descr.getName(), descr)
//{}

template<typename T>
Track<T>::Accessor::Accessor(w4::cref<Track<T>> track)
    : m_track(track)
    , m_currentValueIndex(0)
    , m_nextValueIndex(0)
    , m_deltaTime(0.f)
    , m_isValid(track && track->getValuesCount())
{
}

template<typename T>
Track<T>::Accessor::~Accessor()
{
    m_track->removeAccessor(this);
}

template<typename T>
void Track<T>::Accessor::invalidate()
{
    m_isValid = false;
    m_currentValueIndex = 0;
    m_nextValueIndex = 0;
    m_deltaTime = 0.f;
}

template<typename T>
void Track<T>::Accessor::updateTime(float time)
{
    const auto& values = m_track->getValues();
    if (values.empty())
    {
        return;
    }

    m_nextValueIndex = std::min(m_currentValueIndex + 1, values.size() - 1);
    if(values[m_nextValueIndex].time < time)
    {
        auto maxIndex = values.size() - 1;
        do
        {
            m_currentValueIndex++;
        } while(m_currentValueIndex < maxIndex && values[m_currentValueIndex + 1].time < time);
    }
    else {
        if (values[m_currentValueIndex].time > time) {
            do {
                m_currentValueIndex--;
            } while (values[m_currentValueIndex].time > time && m_currentValueIndex > 0);
        }
    }

    m_currentValueIndex = std::min(std::max(m_currentValueIndex, static_cast<size_t>(0)), values.size() - 2);
    m_nextValueIndex = std::min(m_currentValueIndex + 1, values.size() - 1);

    if (m_currentValueIndex == m_nextValueIndex)
    {
        m_deltaTime = 0.0f;
    } else {
        m_deltaTime =
                std::clamp((time - values[m_currentValueIndex].time) / (values[m_nextValueIndex].time - values[m_currentValueIndex].time), 0.0f, 1.0f);
    }
}

template<typename T>
bool Track<T>::Accessor::isValid() const
{
    return m_isValid;
}

template<typename T>
bool Track<T>::Accessor::isInRange(float time) const
{
    return m_track->isInRange(time);
}

template<typename T>
float Track<T>::Accessor::getDuration() const
{
    return m_track->getDuration();
}

template<typename T>
w4::sptr<Track<T>>& Track<T>::Accessor::getTrack()
{
    return m_track;
}

template<typename T>
T Track<T>::Accessor::getValue() const
{
    const auto& values = m_track->getValues();

    switch (values[m_currentValueIndex].interp)
    {
        case InterpType::CONST:
        {
            return ConstInterpolate<T>::interpolate(values[m_currentValueIndex], values[m_nextValueIndex], m_deltaTime);
        }
        case InterpType::LERP:
        {
            return LerpInterpolate<T>::interpolate(values[m_currentValueIndex], values[m_nextValueIndex], m_deltaTime);
        }
        case InterpType::HERMIT:
        {
            return HermitInterpolate<T>::interpolate(values[m_currentValueIndex], values[m_nextValueIndex], m_deltaTime);
        }
        case InterpType::CUBIC:
        {
            return CubicInterpolate<T>::interpolate(getValueByOffset(-2), getValueByOffset(-1), getValueByOffset(0), getValueByOffset(1), m_deltaTime);
        }
    };

    return T();
}

template<typename T>
typename Track<T>::Value Track<T>::Accessor::getValueByOffset(int offset) const
{
    const auto& values = m_track->getValues();
    const auto newIndex = static_cast<int>(m_currentValueIndex) + offset;
    if(values.empty())
    {
        return Track<T>::Value();
    }
    if(values.size() == 1)
    {
        return values[m_currentValueIndex];
    }
    if(newIndex < 0)
    {
        return values[values.size() + newIndex];
    }
    else if(m_currentValueIndex == (values.size() - 1) && offset == 1)
    {
        // never happens
        return values[0]; // skip 0
    }
    else if(newIndex >= values.size())
    {
        return values[newIndex % values.size()];
    }
    else
    {
        return values[newIndex];
    }
}

template<typename T>
typename Track<T>::Accessor::sptr Track<T>::createAccessor()
{
    auto res = typename Track<T>::Accessor::sptr(new typename Track<T>::Accessor(w4::cast_sptr<Track<T>>(Track<T>::shared_from_this())));
    m_accessors.insert(res.get());
    return res;
}

template<typename T>
Track<T>::Track(const ResourceLoadDescr& descr)
        : w4::resources::Resource(descr.getName(), descr)
{

}

template<typename T>
bool Track<T>::isInRange(float time) const
{
    return m_values.size() == 0 ? false : time < m_values.back().time;
}

template<typename T>
float Track<T>::getDuration() const
{
    return m_values.size() == 0 ? 0 : m_values.back().time;
}

template<typename T>
size_t Track<T>::getValuesCount() const
{
    return m_values.size();
}

template<typename T>
size_t Track<T>::getAccessorsCount() const
{
    return m_accessors.size();
}

template<typename T>
void Track<T>::addValue(const Track<T>::Value& value)
{
    m_values.insert(std::upper_bound(m_values.begin(), m_values.end(), value), value);

    invalidateAccessors();
}

template<typename T>
void Track<T>::removeValues(float fromTime, float toTime)
{
    if (m_values.empty())
    {
        return;
    }
    auto lb = std::lower_bound(m_values.begin(), m_values.end(), TrackValue{fromTime, T(), InterpType::CONST} );
    auto ub = std::upper_bound(lb, m_values.end(), TrackValue{toTime, T(), InterpType::CONST});
    m_values.erase(lb, ub);

    invalidateAccessors();
}

template<typename T>
void Track<T>::removeValues(float fromTime)
{
    if (m_values.empty())
    {
        return;
    }
    removeValues(fromTime, m_values.back().time);
}

template<typename T>
const typename Track<T>::Values& Track<T>::getValues() const
{
    return m_values;
}

template<typename T>
void Track<T>::setValues(Values && values)
{
    m_values.swap(values);
}

template<typename T>
void Track<T>::removeAccessor(Accessor* accessor)
{
    m_accessors.erase(accessor);
}

template<typename T>
void Track<T>::invalidateAccessors()
{
    for (auto& accessor : m_accessors)
    {
        accessor->invalidate();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interpolators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
T ConstInterpolate<T>::interpolate(const typename Track<T>::Value& currentValue, const typename Track<T>::Value& nextValue, float delta)
{
    return currentValue.value;
}

template <typename T>
T LerpInterpolate<T>::interpolate(const typename Track<T>::Value& currentValue, const typename Track<T>::Value& nextValue, float delta)
{
    return w4::math::lerp(currentValue.value, nextValue.value, delta);
}

template <typename T>
T HermitInterpolate<T>::interpolate(const typename Track<T>::Value& currentValue, const typename Track<T>::Value& nextValue, float delta)
{
    return w4::math::hermit(currentValue.value, nextValue.value, currentValue.tangents[1], nextValue.tangents[0], delta);
}

template<typename T>
T CubicInterpolate<T>::interpolate(const typename Track<T>::Value &a,
                                   const typename Track<T>::Value &b,
                                   const typename Track<T>::Value &c,
                                   const typename Track<T>::Value &d,
                                   float delta)
{
    return w4::math::cubic(a.value, b.value, c.value, d.value, delta);
}

} //namespace