#pragma once

#include <cinttypes>
#include <string>
#include <unordered_map>

#include "W4Common.h"

namespace w4::core {

class Rest
{
public:
    enum class Method : uint8_t
    {
        GET = 1,
        POST,
        PUT,
        DELETE
    };

    using Callback = std::function<void(uint16_t, const std::string&)>;

    static void send(Method, const std::string& uri, const Callback&);
    static void send(Method, const std::string& uri, const std::string& headers, const std::string& body, const Callback&);
    static void onSend(void* ctx, uint16_t status, const char* str);
private:
    static std::unordered_map<void*, w4::sptr<Callback>> m_callback;
};

} // namespace w4::core
