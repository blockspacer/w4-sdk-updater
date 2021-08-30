 #pragma once

#include <functional>
#include <memory>
#include <string>

#include "W4Common.h"
#include "W4Math.h"
#include "TaskPool.h"
#include "Input.h"
#include "Event.h"

namespace w4::event
{
    struct Leave  : public w4::event::EventImpl<Leave,  w4::event::Event> { W4_TYPEINFO(Leave,  "Event", Event) };
    struct Resume : public w4::event::EventImpl<Resume, w4::event::Event> { W4_TYPEINFO(Resume, "Event", Event) };
}

namespace w4::platform {

void showLoader();
void hideLoader();
const math::size& getSize();

using DeviceId = std::string;
struct SystemInfo
{
    uint  DPI;
};

class Platform
{
public:
    static void init(const char *parentObject);

    static const SystemInfo& getSystemInfo();
    static Input&            getInput()     ;
    static const DeviceId&   getDeviceId()  ;
    static const math::size& getSize()      ;
    static uint8_t           getContextVersion();

    static void setWindowStyle(const char*);

    static void setWindowAspect (int w, int h);
    static void jumpToURL(const std::string& url);

    static void setLoaderStyle(const char*);
    static void showLoader();
    static void hideLoader();
private:
    static void onLost();
    static void onRestore();
    static void onResume();
    static void onLeave();
    static void onPause();
    static void onResize(uint, uint);

    static math::size  m_size;
    static SystemInfo  m_info;

    static uint8_t m_contextVersion;
};



} // namespace

