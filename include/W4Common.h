#pragma once

#include <memory>
#include <string>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #define W4_JS_IMPORT extern "C"
#endif

using uint = unsigned int;

#ifdef _MSC_VER
    #ifdef WIN_CLANG_COMPILER
        using literal_int = unsigned int;
    #else
        using literal_int = unsigned long long int;
    #endif
#else
    using literal_int = unsigned long int;
#endif



namespace w4
{
    template<typename T> using sptr = std::shared_ptr<T>;
    template<typename T> using uptr = std::unique_ptr<T>;
    template<typename T> using wptr = std::weak_ptr<T>;
    template<typename T> using cref = const sptr<T>&;
    template<typename Tdst, typename Tsrc> sptr<Tdst> cast_sptr(cref<Tsrc>& op) {return std::static_pointer_cast<Tdst>(op);};
    template<typename T> struct enable_from_this : public std::enable_shared_from_this<T> {};

}

namespace w4::make
{
    template<typename T, typename ...Args> w4::sptr<T> sptr(Args&& ...args) { return std::make_shared<T>(std::forward<Args>(args)...); }
    template<typename T, typename ...Args> w4::uptr<T> uptr(Args&& ...args) { return std::make_unique<T>(std::forward<Args>(args)...); }
}

namespace std
{
    template <typename T1, typename T2>
    struct hash<std::pair<T1, T2>>
    {
        std::size_t operator()(const std::pair<T1, T2>& p) const
        {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);

            return h1 ^ h2;
        }
    };
} // std

