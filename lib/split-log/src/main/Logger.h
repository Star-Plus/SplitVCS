#pragma once
#include <string>
#include <chrono>
#include "utils/Color.h"

namespace Split {

    class Logger {
    public:
        Logger();
        explicit Logger(bool debugMode, const std::string& name="");

        void info(const std::string& message) const;
        void warning(const std::string& message) const;
        void error(const std::string& message) const;
        void debug(const std::string& message) const;

        void setCheckPoint(const std::string& message="");

        void log(const std::string& message, Color color = Color::RESET) const;

        void setDebugMode(bool enabled);

    private:
        bool debugMode = false;
        std::string name;
        std::chrono::steady_clock::time_point checkPointTime;
    };

}