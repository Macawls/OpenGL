#pragma once

#include <iostream>
#include <mutex>
#include <ctime>
#include <cstdarg>
#include <deque>

/* Example usage:
    Logger::Log(Logger::LogPriority::Error, "Error: %s", glewGetErrorString(err));
    Logger::Log("OpenGL version: %s", glGetString(GL_VERSION));
    Logger::Log("Hello there");
*/

class Logger
{
public:
    enum class LogPriority
    {
        Debug,
        Info,
        Warning,
        Error
    };

#define DEFAULT_PRIORITY LogPriority::Info
#define DEFAULT_MAX_HISTORY_SIZE 50
#define DEFAULT_MESSAGE_BUFFER_SIZE 256

    // Set minimum priority level, default is Info
    static void SetPriority(LogPriority priority)
    {
        get_instance().min_priority = priority;
    }

    // Set maximum number of messages to store in history, default is 50
    static void SetMaxHistorySize(size_t size)
    {
        get_instance().max_history_size = size;
    }

    // Returns a stack of the most recent log messages
    static std::deque<std::string> GetHistory()
    {
        return get_instance().history;
    }

    // Logs a message with a default priority of INFO
    // Output format: "[HH:MM:SS] [Priority] Message"
    template <typename... Args>
    static void Log(const char *format, Args... args)
    {
        Log(DEFAULT_PRIORITY, format, args...);
    }

    template <typename... Args>
    static void LogError(const char *format, Args... args)
    {
        Log(LogPriority::Error, format, args...);
    }

    template <typename... Args>
    static void LogWarning(const char *format, Args... args)
    {
        Log(LogPriority::Warning, format, args...);
    }

    template <typename... Args>
    static void LogDebug(const char *format, Args... args)
    {
        Log(LogPriority::Debug, format, args...);
    }

    // Logs a message with a specified priority
    // Output format: "[HH:MM:SS] [Priority] Message"
    template <typename... Args>
    static void Log(LogPriority priority, const char *format, Args... args)
    {
        if (priority >= get_instance().min_priority)
        {
            std::scoped_lock lock(get_instance().mutex);
            print_timestamp();
            std::cout << get_color_code(priority) << "[" << priority_to_string(priority) << "] " << COLOR_RESET;
            printf(format, args...);
            std::cout << std::endl;

            add_to_history(format, args...);
        }
    }

private:
    LogPriority min_priority = DEFAULT_PRIORITY;
    size_t max_history_size = DEFAULT_MAX_HISTORY_SIZE;

    std::mutex mutex;
    std::deque<std::string> history;
    

    static constexpr const char *COLOR_RED = "\033[31m";
    static constexpr const char *COLOR_YELLOW = "\033[33m";
    static constexpr const char *COLOR_GREEN = "\033[32m";
    static constexpr const char *COLOR_BLUE = "\033[34m";
    static constexpr const char *COLOR_GRAY = "\033[90m";
    static constexpr const char *COLOR_RESET = "\033[0m";

    Logger() {}

    static Logger &get_instance()
    {
        static Logger instance;
        return instance;
    }

    static const char *priority_to_string(LogPriority priority)
    {
        switch (priority)
        {
        case LogPriority::Debug:
            return "Debug";
        case LogPriority::Info:
            return "Info";
        case LogPriority::Warning:
            return "Warning";
        case LogPriority::Error:
            return "Error";
        default:
            return "Unknown";
        }
    }

    static const char *get_color_code(LogPriority priority)
    {
        switch (priority)
        {
        case LogPriority::Debug:
            return COLOR_GREEN;
        case LogPriority::Info:
            return COLOR_BLUE;
        case LogPriority::Warning:
            return COLOR_YELLOW;
        case LogPriority::Error:
            return COLOR_RED;
        default:
            return COLOR_RESET;
        }
    }

    static void print_timestamp()
    {
        std::time_t current_time = std::time(nullptr);
        char time_buffer[9]; // exact size of "HH:MM:SS\0"
        std::strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", std::localtime(&current_time));
        std::cout << COLOR_GRAY << "[" << time_buffer << "] " << COLOR_RESET;
    }

    static void add_to_history(const char *format, ...)
    {
        Logger &logger = get_instance();

        char message_buffer[DEFAULT_MESSAGE_BUFFER_SIZE];
        va_list args;
        va_start(args, format);
        vsnprintf(message_buffer, sizeof(message_buffer), format, args);
        va_end(args);

        std::string message = message_buffer;

        if (logger.history.size() >= logger.max_history_size)
        {
            logger.history.pop_back();
        }
        
        logger.history.push_front(message);
    }
};