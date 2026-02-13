#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

bool Logger::m_initialized = false;

void Logger::initialize()
{
    if (!m_initialized) {
        m_initialized = true;
        info("Logger initialized");
    }
}

void Logger::shutdown()
{
    if (m_initialized) {
        info("Logger shutdown");
        m_initialized = false;
    }
}

void Logger::log(LogLevel level, const std::string& message)
{
    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // 格式化时间
    std::tm tm_now;
    localtime_s(&tm_now, &now_time);
    
    // 输出日志
    std::cout << "[" 
              << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S.") 
              << std::setfill('0') << std::setw(3) << now_ms.count() 
              << "] [" << levelToString(level) << "] " << message << std::endl;

    // 如果是致命错误，终止程序
    if (level == LogLevel::FATAL) {
        std::abort();
    }
}

void Logger::debug(const std::string& message)
{
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message)
{
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message)
{
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message)
{
    log(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message)
{
    log(LogLevel::FATAL, message);
}

std::string Logger::levelToString(LogLevel level)
{
    switch (level) {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::FATAL:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
}
