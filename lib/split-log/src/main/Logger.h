#pragma once
#include <string>
#include "utils/Color.h"

namespace Split {

    class Logger {
    public:
        Logger();
        explicit Logger(bool debugMode);

        void info(const std::string& message);
        void warning(const std::string& message);
        void error(const std::string& message);
        void debug(const std::string& message);

        static void log(const std::string& message, const Color color = Color::RESET);

        void setDebugMode(bool enabled);

    private:
        bool debugMode = false;
    };

}