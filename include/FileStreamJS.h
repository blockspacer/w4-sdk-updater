#pragma once

#include <string>
#include <vector>
#include <iosfwd>

#include "FileStream.h"

namespace w4::filesystem {

class FileStreamJS : public Stream
{
public:
    FileStreamJS(const std::string& path);
    ~FileStreamJS();

    size_t write(const uint8_t* buffer, size_t bufferSize) override;
    size_t read(uint8_t* buffer, size_t bufferSize) override;
    void seek(size_t value) override;
    size_t tell() override;
    size_t size() override;
    bool good() const override;
    const uint8_t* data() override;

    static void syncCallback(void* context, const uint8_t* buffer, size_t bufferSize);

private:
    void sync();


    bool m_good = false;
    bool m_changed = false;
    size_t m_pos = 0;
    const uint8_t* m_buffer;
    size_t m_bufferSize;
};

}