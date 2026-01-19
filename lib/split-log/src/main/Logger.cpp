#include <iostream>
#include <source_location>
#include "Logger.h"
#include "utils/Format.h"


namespace Split {

    Logger::Logger() : debugMode(false) {}
    Logger::Logger(bool debugMode) : debugMode(debugMode) {}

    void Logger::info(const std::string& message) {
        log(message, Color::GREEN);
    }

    void Logger::warning(const std::string& message) {
        log(message, Color::YELLOW);
    }

    void Logger::error(const std::string& message) {
        log(message, Color::RED);
    }

    void Logger::debug(const std::string& message) {
        log(message, Color::CYAN);
    }

    void Logger::log(const std::string& message, const Color color) {

        setConsoleColor(color);

        const std::source_location location = std::source_location::current();
        std::cout << "[" << Format::split(location.file_name(), '\\').back() << ":" << location.line() << "] ";

        std::cout << message << std::endl;
        setConsoleColor(Color::RESET);
    }

    void Logger::setDebugMode(bool enabled) {
        debugMode = enabled;
    }

}