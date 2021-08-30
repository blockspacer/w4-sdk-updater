#pragma once
#include <vector>
#include <string>
#include "W4Common.h"

namespace w4::utils {

    constexpr unsigned int strlen(const char*s, int off = 0) {return !s[off] ? 1 : strlen(s, off+1) + 1;}
    constexpr unsigned int hash(const char *s, int off = 0)  {return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];}

    constexpr unsigned long hash2(const char *str)
    {
        unsigned long hash = 5381;
        while (char c = *str++)
            hash = (hash * 33) ^ c;

        return hash;
    }

    std::vector<std::string> split(const std::string& str, const std::string& delim);


    std::string loadFile(const std::string & path);

    uint32_t crc32(uint32_t crc, unsigned char byte);
    uint32_t crc32(const void* source, size_t size);
    uint32_t crc32(const std::string& str);

    struct CRCWrapper
    {
    public:
        explicit CRCWrapper(const std::string& str);
        explicit CRCWrapper(std::string&& str);

        bool operator==(const CRCWrapper &other) const;
        const std::string& getStr() const;

        friend std::hash<w4::utils::CRCWrapper>;

    private:
        uint32_t m_crc;
        std::string m_str;
    };

    struct UIDGenerator
    {
        static size_t get();
    };

    void InitW4Lib();

    std::string base64_encode(const std::string& str);
    std::string base64_encode(const char* data, unsigned int len);
    std::string base64_decode(std::string const& s);
}


namespace std
{
    template <>
    struct hash<w4::utils::CRCWrapper>
    {
        std::size_t operator()(const w4::utils::CRCWrapper& k) const
        {
            return k.m_crc;
        }
    };
}