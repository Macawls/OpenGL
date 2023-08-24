#include <iostream>
#include <mutex>
#include <ctime>

class Logger {
public:
    enum class LogPriority {
        Debug, Info, Warning, Error
    };

    // minimum priority level
    static void SetPriority(LogPriority priority) {
        get_instance().min_priority = priority;
    }

    // Log a message with a specified priority
    template <typename... Args>
    static void Log(LogPriority priority, const char* format, Args... args) {
    if (priority >= get_instance().min_priority) {
        std::scoped_lock lock(get_instance().mutex);
        print_timestamp();
        std::cout << get_color_code(priority) << "[" << priority_to_string(priority) << "] " << COLOR_RESET;
        printf(format, args...);
        std::cout << std::endl;
    }
}

private:
    LogPriority min_priority = LogPriority::Info;
    std::mutex mutex;

    static constexpr const char* COLOR_RED = "\033[31m";
    static constexpr const char* COLOR_YELLOW = "\033[33m";
    static constexpr const char* COLOR_GREEN = "\033[32m";
    static constexpr const char* COLOR_BLUE = "\033[34m";
    static constexpr const char* COLOR_GRAY = "\033[90m";
    static constexpr const char* COLOR_RESET = "\033[0m";

    Logger() {}

    static Logger& get_instance() {
        static Logger instance;
        return instance;
    }

    static const char* priority_to_string(LogPriority priority) {
        switch (priority) {
            case LogPriority::Debug: return "Debug";
            case LogPriority::Info: return "Info";
            case LogPriority::Warning: return "Warning";
            case LogPriority::Error: return "Error";
            default: return "Unknown";
        }
    }

    static const char* get_color_code(LogPriority priority) {
        switch (priority) {
            case LogPriority::Debug: return COLOR_GREEN;
            case LogPriority::Info: return COLOR_BLUE;
            case LogPriority::Warning: return COLOR_YELLOW;
            case LogPriority::Error: return COLOR_RED;
            default: return COLOR_RESET;
        }
    }

    static void print_timestamp() {
        std::time_t current_time = std::time(nullptr);
        char time_buffer[9]; //exact size of "HH:MM:SS\0"
        std::strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", std::localtime(&current_time));
        std::cout << COLOR_GRAY << "[" << time_buffer << "] " << COLOR_RESET;
    }
};