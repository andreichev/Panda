#pragma once

#include <string>
#include <format>

namespace Bamboo {

class Logger {
public:
    enum MessageType { INFO = 0, WARNING, ERROR };
    static void log(MessageType type, std::string message);

    template<typename... Args>
    static void logF(MessageType type, std::format_string<Args...> fstr, Args &&...args) {
        std::string message = std::format(fstr, std::forward<Args>(args)...);
        log(type, message);
    }
};

} // namespace Bamboo

#define LOG_INFO(...) Bamboo::Logger::logF(Bamboo::Logger::MessageType::INFO, __VA_ARGS__)
#define LOG_WARN(...) Bamboo::Logger::logF(Bamboo::Logger::MessageType::WARNING, __VA_ARGS__)
#define LOG_ERROR(...) Bamboo::Logger::logF(Bamboo::Logger::MessageType::ERROR, __VA_ARGS__)