#pragma once

#include <string>
#include <array>

namespace w4 {

class Uuid
{
public:
    static Uuid generate();
    static Uuid zero();

    std::string toString() const;

private:
    std::array<uint8_t, 16> m_data;
};

} //namespace w4