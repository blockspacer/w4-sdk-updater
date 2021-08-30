#pragma once

#include "W4Common.h"
#include <cstdint>

namespace w4::gui
{

enum class Font
{
    CourierNew = 0,
    Arial = 1,
    Times = 2,
    Copperplate = 3,
    Helvetica = 4,
    FreeMono = 5,
    ComicSans = 6,
    Tahoma   = 7,
    Lato     = 8
};

}

namespace std
{
    string to_string(w4::gui::Font font);
}

