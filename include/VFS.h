#pragma once

#include <string>
#include <memory>
#include <unordered_set>
#include <functional>

#include "FileStream.h"
#include "W4Common.h"



namespace w4::filesystem {

/// Open file and return FileStream on it's data
/// \param path - path to the file
/// \param mode - FileMode
/// \return shared ptr to Stream
w4::sptr<Stream> open(const std::string& path, const FileMode& mode = FileMode::Read);

using FetchCallback  = std::function<void(const w4::sptr<Stream>&, const Status&)>;

void fetch(const std::string& path, const FetchCallback& callback);

#ifndef __EMSCRIPTEN__
bool mkdir(const std::string& name);
bool rmdir(const std::string& name);
#endif

bool isPathExists(const std::string& path);

class VFS
{
public:
    using Callback  = std::function<void()>;

    /// VFS singleton instantination
    /// \return virtual filesystem's singleton reference
    static VFS& instance();

    /// Platform dependent virtual filesystem initialisation
    /// \param path - native path to config file
    /// \param callback - callback, that will be called after VFS initialisation
    void init(const Callback& callback);

    /// Platform dependent virtual filesystem clean
    /// \param path
    /// \param callback
    void clean(const Callback& callback);


    static void initCallback();

    static void cleanCallback();

    ~VFS();

    friend w4::sptr<Stream> open(const std::string& path, const FileMode& mode);
    friend void fetch(const std::string& path, const FetchCallback& callback);
private:

    /// Open file and return FileStream on it's data
    /// \param path - path to the file
    /// \param mode - FileMode
    /// \return shared ptr to FileStream
    w4::sptr<Stream> open(const std::string& path, const FileMode& mode = FileMode::Read);

    void fetch(const std::string& path, const FetchCallback& callback);

    VFS() = default;

    Callback m_initCallback;
    Callback m_cleanCallback;

    std::unordered_set<Stream*> m_fetchedStreams;
};

}