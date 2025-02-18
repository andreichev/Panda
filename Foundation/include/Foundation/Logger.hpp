//
// Created by Admin on 03.02.2022.
//

#pragma once

#include <string_view>
#include <memory>
#include <cstdarg>

namespace Foundation {

class Logger {
public:
    enum class MessageType { _TRACE, _INFO, _WARNING, _ERROR, _CRITICAL };
    static void init();

    template<typename... Args>
    static void log(MessageType type, const char *format, ...) {
        static const int32_t maxSize = 256;
        static char buffer[maxSize];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        std::string_view view = buffer;
        log(view, type);
    }

private:
    enum class ColorType { DEFAULT, GREEN, YELLOW, RED };
    enum class ColorBgType { DEFAULT, RED };
    struct TextInfo {
        ColorType colorType = ColorType::DEFAULT;
        ColorBgType colorBgType = ColorBgType::DEFAULT;
        bool bold = false;
    };
    static void log(std::string_view message, MessageType type);
    static void log(std::string_view message);
    static void setTextAttrib(TextInfo textInfo);
    static void reset();
};

class EditorLogger {
public:
    enum MessageType { INFO, WARNING, ERROR };
    using EditorLoggerCallback = void (*)(std::string_view message, MessageType type);

    static void init(EditorLoggerCallback callback);
    static void log(std::string_view message, MessageType type);

    template<typename... Args>
    static void log(MessageType type, const char *format, ...) {
        static const int32_t maxSize = 256;
        static char buffer[maxSize];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        std::string_view view = buffer;
        log(view, type);
    }

private:
    static EditorLoggerCallback s_callback;
};

} // namespace Foundation

#define LOG_TRACE(...)                                                                             \
    ::Foundation::Logger::log(Foundation::Logger::MessageType::_TRACE, __VA_ARGS__)
#define LOG_INFO(...) ::Foundation::Logger::log(Foundation::Logger::MessageType::_INFO, __VA_ARGS__)
#define LOG_WARN(...)                                                                              \
    ::Foundation::Logger::log(Foundation::Logger::MessageType::_WARNING, __VA_ARGS__)
#define LOG_ERROR(...)                                                                             \
    ::Foundation::Logger::log(Foundation::Logger::MessageType::_ERROR, __VA_ARGS__)
#define LOG_CRITICAL(...)                                                                          \
    ::Foundation::Logger::log(Foundation::Logger::MessageType::_CRITICAL, __VA_ARGS__)

#define LOG_INFO_EDITOR(...)                                                                       \
    ::Foundation::EditorLogger::log(Foundation::EditorLogger::MessageType::INFO, __VA_ARGS__)
#define LOG_WARN_EDITOR(...)                                                                       \
    ::Foundation::EditorLogger::log(Foundation::EditorLogger::MessageType::WARNING, __VA_ARGS__)
#define LOG_ERROR_EDITOR(...)                                                                      \
    ::Foundation::EditorLogger::log(Foundation::EditorLogger::MessageType::ERROR, __VA_ARGS__)