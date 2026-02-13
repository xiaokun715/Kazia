#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger
{
private:
    static bool m_initialized;

public:
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    static void initialize();
    static void shutdown();

    static void log(LogLevel level, const std::string& message);
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    static void fatal(const std::string& message);

private:
    static std::string levelToString(LogLevel level);
};

#endif // LOGGER_H
