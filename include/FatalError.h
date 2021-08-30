#pragma once

#include "W4Logger.h"

namespace w4
{

    typedef void (*TestCallback)(void *, const char *);

    void internal_assert(const char *, const char *, int, const char *);
    void abort(const std::string&);
    void reload();



}
#define FATAL_ERROR(FORMAT, ...) w4::abort(w4::utils::format(FORMAT, ## __VA_ARGS__))

#ifdef NDEBUG
    #define	W4_ASSERT(x) (void)0
#else
    #define W4_ASSERT(x) ((void)((x) || (w4::internal_assert(#x, __FILE__, __LINE__, __func__),0)))
#endif

#undef assert
#define assert W4_ASSERT
