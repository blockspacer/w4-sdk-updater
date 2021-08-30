#pragma once

#include "W4Common.h"
#include "Object.h"
#include "W4Logger.h"
#include "Utils.h"
#include "TypeInfo.h"

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <queue>

namespace w4::event {

class Handle
{
public:
    using id_t = uint32_t;

    explicit Handle(id_t id)
        : m_id(id)
    {}
    virtual void unsubscribe();
    id_t id() const { return m_id; }
    virtual ~Handle() {}
protected:
    id_t m_id;
};

class Event
{
public:
    using Handler = std::function<void(const Event &)>;

    Event() = default;
    Event(const Event &) = default;

    virtual ~Event() = default;

    static w4::sptr<Handle> subscribe(const Handler&);
    static void unsubscribe(Handle::id_t);
    static void performEmitAll();

    static constexpr core::TypeInfo typeInfo{"Event"};
    virtual const core::TypeInfo& getTypeInfo() const = 0;

protected:
    static void   performEmit(const Event &);
    static inline std::vector<std::function<void ()>> emitters;
    static inline std::queue<size_t> emitterQueue;
private:
    static inline Handle::id_t currentIdx = 0;
    static inline std::unordered_map<Handle::id_t, Handler> handlers;
    static inline std::unordered_set<Handle::id_t> handlerErased;
};

template <typename T, typename D>
class EventImpl : public D
{
public:
    using Super = D;
    using Base = EventImpl<T, D>;

    using cref    = const T &;
    using Handler = std::function<void(cref)>;
    using TypeId  = uint32_t;

    class Handle: public w4::event::Handle
    {
    public:
        using Id = uint32_t;
        using sptr = w4::sptr<Handle>;
        explicit Handle(Id id) : w4::event::Handle(id) { }
        void unsubscribe() override;
    };

    template <typename... Args>
    EventImpl(Args&&... params) : D(std::forward<Args>(params)...) { static_assert(std::is_base_of_v<Event, T>, "T must be subclass of Event"); }

    static typename EventImpl<T, D>::Handle::sptr subscribe(const Handler&);
    static void   unsubscribe(typename Handle::id_t);
    static void   fire(cref);
    //static TypeId getId();

protected:
    static void performEmit(cref);
    static void performEmitFirst();

    struct Internal
    {
        static Internal& instance()
        {
            static Internal m_instance;
            return m_instance;
        }

        Internal()
        {
            emitterIdx = Event::emitters.size();
            Event::emitters.push_back(&Base::performEmitFirst);
        }
        typename Handle::id_t                               currentIdx = 0;
        std::queue<T>                                       events;
        std::unordered_set<typename Handle::id_t>           handlerErased;
        std::map<typename Handle::id_t, Handler>            handlers;
        size_t                                              emitterIdx;
    };

};



#include "impl/Event.inl"
}
