#pragma once
#include <string>

namespace Split {
    enum class Color {
        RED,
        GREEN,
        BLUE,
        YELLOW,
        CYAN,
        MAGENTA,
        WHITE,
        BLACK,
        RESET
    };

    void setConsoleColor(Color color);
}