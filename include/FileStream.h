#pragma once

#include <string>
#include <memory>
#include <regex>

#include <iosfwd>

namespace w4::filesystem {

enum class  FileMode
{
    Read,
    Write,
    Append
};

class Stream
{
public:
    Stream(const std::string& path, const FileMode& mode)
    : m_path(path)
    , m_mode(mode)
    {}

    virtual ~Stream() {}

    Stream(const Stream&) = delete;
    Stream& operator=(const Stream&) = delete;

    virtual size_t write(const uint8_t* buffer, size_t bufferSize) = 0;
    virtual size_t read(uint8_t* bufffer, size_t bufferSize) = 0;
    virtual void seek(size_t pos) = 0;
    virtual size_t tell() = 0;
    virtual size_t size() = 0;
    virtual bool good() const = 0;
    virtual const uint8_t* data() = 0;

    const std::string& getPath() const { return m_path; }
    const FileMode& getMode() const { return m_mode; }

private:
    std::string m_path;
    FileMode m_mode;
};

enum Status
{
    Success = 0,
    Rejected = 1,
    Aborted = 2,
    Error = 3
};


}

