#pragma once

//! @{
#define W4_LOG_LEVEL_ALL     -1 //!< to log literally everything possible
#define W4_LOG_LEVEL_TRACE    0 //!< Brief description after the member
#define W4_LOG_LEVEL_DEBUG    1 //!< only debug things here
#define W4_LOG_LEVEL_INFO     2 //!< only info things here
#define W4_LOG_LEVEL_WARNING  3 //!< only warning things here
#define W4_LOG_LEVEL_ERROR    4 //!< only error things here
#define W4_LOG_LEVEL_NONE     5 //!< just shut up
//! @}

#ifndef W4_LOG_MIN_LEVEL
    #ifdef NDEBUG
        #if defined __EMSCRIPTEN__
            #define W4_LOG_MIN_LEVEL W4_LOG_LEVEL_ERROR
        #else
            #define W4_LOG_MIN_LEVEL W4_LOG_LEVEL_INFO
        #endif
    #else
        #define W4_LOG_MIN_LEVEL W4_LOG_LEVEL_ALL
    #endif
#endif

#include <string>
#include <memory>

namespace w4::utils
{
    const char* unsafe_fmt(const char* fmt, ... );

    template<typename ... Args>
    std::string format(const char* fmt, Args && ...args)
    {
        return std::string(unsafe_fmt(fmt, args...));
    }
}

namespace w4::logger
{
    void initialize(const std::string &deviceId, const std::string &remoteAddress);
    void log(const uint8_t logLevel, char const *const file, int const line, char const *const message);
    void pushIndentLevel();
    void popIndentLevel();

    struct AutoPushPopIndentLevel
    {
        AutoPushPopIndentLevel() { pushIndentLevel(); }
        ~AutoPushPopIndentLevel() { popIndentLevel(); }
    };
}


#if W4_LOG_MIN_LEVEL <= W4_LOG_LEVEL_TRACE
    #define W4_LOG_TRACE(FORMAT, ...) w4::logger::log(W4_LOG_LEVEL_TRACE, __FILE__, __LINE__, w4::utils::unsafe_fmt(FORMAT, ##__VA_ARGS__))
#else
    #define W4_LOG_TRACE(FORMAT, ... )
#endif

#if W4_LOG_MIN_LEVEL <= W4_LOG_LEVEL_DEBUG
    #define W4_LOG_DEBUG(FORMAT, ...) w4::logger::log(W4_LOG_LEVEL_DEBUG, __FILE__, __LINE__, w4::utils::unsafe_fmt(FORMAT, ##__VA_ARGS__))
#else
    #define W4_LOG_DEBUG(FORMAT, ...)
#endif

#if W4_LOG_MIN_LEVEL <= W4_LOG_LEVEL_INFO
    #define W4_LOG_INFO(FORMAT, ...) w4::logger::log(W4_LOG_LEVEL_INFO, __FILE__, __LINE__, w4::utils::unsafe_fmt(FORMAT, ##__VA_ARGS__))
#else
    #define W4_LOG_INFO(FORMAT, ...)
#endif

#if W4_LOG_MIN_LEVEL <= W4_LOG_LEVEL_WARNING
    #define W4_LOG_WARNING(FORMAT, ...) w4::logger::log(W4_LOG_LEVEL_WARNING, __FILE__, __LINE__, w4::utils::unsafe_fmt(FORMAT, ##__VA_ARGS__))
#else
    #define W4_LOG_WARNING(FORMAT, ...)
#endif

#if W4_LOG_MIN_LEVEL <= W4_LOG_LEVEL_ERROR
    #define W4_LOG_ERROR(FORMAT, ...) w4::logger::log(W4_LOG_LEVEL_ERROR, __FILE__, __LINE__, w4::utils::unsafe_fmt(FORMAT, ##__VA_ARGS__))
#else
    #define W4_LOG_ERROR(FORMAT, ...)
#endif

#if W4_LOG_MIN_LEVEL > W4_LOG_LEVEL_ERROR
    #define W4_LOG_PUSH_INDENT
    #define W4_LOG_POP_INDENT
    #define W4_LOG_AUTO_PUSH_INDENT
#else
    #define W4_LOG_PUSH_INDENT w4::logger::pushIndentLevel()
    #define W4_LOG_POP_INDENT w4::logger::popIndentLevel()

    #define W4_LOG_AUTO_PUSH_INDENT w4::logger::AutoPushPopIndentLevel __AutoPushLogLevel__
#endif
