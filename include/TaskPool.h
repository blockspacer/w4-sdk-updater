#pragma once

#include <unordered_map>
#include <functional>

namespace w4::core {

template<typename Proto>
class TaskPool
{};

template<typename... Args>
class TaskPool<void (Args...)>
{
public:
    using Callable = std::function<bool (Args...)>;
    using Handle = uint32_t;

    TaskPool() = default;

    void  emit(Args... args);
    Handle addCallable(const Callable& callable);
    bool removeByHndl(Handle hndl);
    bool isValidByHndl(Handle hndl) const;

    void reset();
    bool isEmpty() const;

private:
    struct Task
    {
        Callable m_func;
        bool m_isValid;
    };

    Handle m_currentIdx = 0;

    std::unordered_map<Handle, Task> m_tasks;
    std::unordered_map<Handle, Task> m_addedTasks;
};

#include "impl/TaskPool.inl"

}