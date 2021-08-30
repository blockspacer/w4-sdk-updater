#pragma once

#include <string>
#include <unordered_map>

#include "W4Common.h"

namespace w4::social::api {

class OK
{
public:
    using StrCallback = std::function<void(const std::string&)>;
    using BoolCallback = std::function<void(bool)>;

    static void getID(const StrCallback&);
    static void getName(const StrCallback&);
    // if default OK user pic will be returned, u will catch CORS trying to set it as image
    static void getPhoto(const StrCallback&);
    static void getAppUsers(const StrCallback&);
    static void loadAd(const BoolCallback&);
    static void showLoadedAd(const BoolCallback&);
    static void postMediatopic(const BoolCallback&);
    static void checkPermission(const std::string&, const BoolCallback&);
    static void askPermission(const std::string&, const BoolCallback&);
    static void suggestDesktopIcon(const BoolCallback&);

    static void onStrCallback(void* ctx, const char* str);
    static void onBoolCallback(void* ctx, bool flag);

private:
    static void* registerCallback(const StrCallback&);
    static void* registerCallback(const BoolCallback&);

    static std::unordered_map<void*, w4::sptr<StrCallback>> m_strCallbacks;
    static std::unordered_map<void*, w4::sptr<BoolCallback>> m_boolCallbacks;
};

}
