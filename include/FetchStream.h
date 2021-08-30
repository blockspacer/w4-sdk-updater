#pragma once

#include <string>
#include <vector>
#include <iosfwd>

#include "FileStream.h"



namespace w4::filesystem {

class FetchStream : public Stream
{
public:

    using Callback  = std::function<void(Stream*, const Status& status)>;

    FetchStream(const std::string& path, const Callback&);
    ~FetchStream();

    size_t write(const uint8_t* buffer, size_t bufferSize) override;
    virtual size_t read(uint8_t* buffer, size_t bufferSize) override;
    virtual void seek(size_t value) override;
    virtual size_t tell() override;
    virtual size_t size() override;
    bool good() const override;
    const uint8_t* data() override;


private:
    void sync();
    static void syncCallback(void* context, uint8_t* buffer, size_t bufferSize, uint32_t status, const char* msg);

    Callback m_fetch_cb;

    bool m_good = false;
    bool m_changed = false;
    size_t m_pos = 0;
    uint8_t* m_buffer;
    size_t  m_size = 0;
};

}