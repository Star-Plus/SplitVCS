#include <iostream>
#include <source_location>
#include "Logger.h"
#include "utils/Format.h"


namespace Split {

    Logger::Logger()
    {
        setCheckPoint();
    }
    Logger::Logger(const bool debugMode, const std::string& name) : debugMode(debugMode), name(name)
    {
        setCheckPoint();
    }

    void Logger::info(const std::string& message) const {
        log(message, Color::GREEN);
    }

    void Logger::warning(const std::string& message) const {
        log(message, Color::YELLOW);
    }

    void Logger::error(const std::string& message) const {
        log(message, Color::RED);
    }

    void Logger::debug(const std::string& message) const {
        log(message, Color::CYAN);
    }


    void Logger::setCheckPoint(const std::string& message)
    {
        checkPointTime = std::chrono::high_resolution_clock::now();
        info("Set an checkout point " + message);
    }

    void Logger::log(const std::string& message, const Color color) const {

        setConsoleColor(color);

        const auto currentTime = std::chrono::high_resolution_clock::now();

        const auto duration = std::chrono::duration<double, std::milli>(currentTime - checkPointTime).count();

        const std::source_location location = std::source_location::current();

        std::cout << "[" << Format::split(location.file_name(), '\\').back() << ":" << location.line() << "] " << "(" << name << ") " << "[" << static_cast<int>(duration) << " ms] " ;

        std::cout << message << std::endl;
        setConsoleColor(Color::RESET);

    }

    void Logger::setDebugMode(const bool enabled) {
        debugMode = enabled;
    }

}