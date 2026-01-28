#include <iostream>
#include <source_location>
#include <utility>
#include "Logger.h"
#include "utils/Format.h"


namespace Split {

    Logger::Logger()
    {
        logFile = std::fstream("logs/ALL.log", std::ios::out | std::ios::app);
        setCheckPoint();
    }
    Logger::Logger(const bool debugMode, std::string name) : debugMode(debugMode), name(std::move(name))
    {
        const auto logFileName = name.empty() ? "ALL.log" : name + ".log";
        logFile = std::fstream("logs/" + logFileName, std::ios::out | std::ios::app);
        setCheckPoint();
    }

    Logger::~Logger()
    {
        logFile.close();
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
        checkPointTime = std::chrono::steady_clock::now();
        info("Set an checkout point " + message);
    }

    void Logger::log(const std::string& message, const Color color) const {
        const auto currentTime = std::chrono::steady_clock::now();

        const auto duration = std::chrono::duration<double, std::milli>(currentTime - checkPointTime).count();

        const std::source_location location = std::source_location::current();

        logFile << "[" << Format::split(location.file_name(), '\\').back()
        << ":" << location.line() << "] "
        << "(" << name << ") "
        << "[" << static_cast<int>(duration) << " ms] "
        << message << "\n";
    }

    void Logger::setDebugMode(const bool enabled) {
        debugMode = enabled;
    }

}