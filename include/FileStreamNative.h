#pragma once

#include <fstream>
#include <vector>
#include "FileStream.h"

namespace w4::filesystem {

class FileStreamNative : public Stream
{
public:
    FileStreamNative(const std::string& path, const FileMode& mode);

    size_t write(const uint8_t* buffer, size_t bufferSize) override;
    size_t read(uint8_t* buffer, size_t bufferSize) override;
    void seek(size_t pos) override;
    size_t tell() override;
    size_t size() override;
    bool good() const override;
    const uint8_t* data() override;

private:
    std::fstream m_stream;
    std::vector<uint8_t> m_buffer;
};

}

