#pragma once

#include <string>
#include <format>

class Logger {
public:
    static void log(std::string message);

    template<typename... Args>
    static void logF(std::format_string<Args...> fstr, Args &&...args) {
        std::string message = std::format(fstr, std::forward<Args>(args)...);
        log(message);
    }
};

#define LOG(...) ::Logger::logF(__VA_ARGS__)