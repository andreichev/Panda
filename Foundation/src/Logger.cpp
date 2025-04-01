//
// Created by Admin on 03.02.2022.
//

#include "Foundation/Logger.hpp"
#include "Foundation/PlatformDetection.hpp"
#include <sstream>

#ifdef PLATFORM_WINDOWS
#    include <windows.h>
#endif

static std::string getTimeToString(time_t &time) {
    std::tm *tm = std::localtime(&time);
    auto h = tm->tm_hour, m = tm->tm_min, s = tm->tm_sec;
    std::stringstream res;
    if (h < 10) {
        res << '0' << h;
    } else {
        res << h;
    }
    res << ':';
    if (m < 10) {
        res << '0' << m;
    } else {
        res << m;
    }
    res << ':';
    if (s < 10) {
        res << '0' << s;
    } else {
        res << s;
    }
    return res.str();
}

namespace Foundation {

EditorLogger::EditorLoggerCallback EditorLogger::s_callback;

void Logger::log(std::string_view message, Logger::MessageType type) {
    TextInfo textInfo;
    switch (type) {
        case Logger::MessageType::_TRACE:
            break;
        case Logger::MessageType::_INFO:
            textInfo.colorType = ColorType::GREEN;
            break;
        case Logger::MessageType::_WARNING:
            textInfo.colorType = ColorType::YELLOW;
            break;
        case Logger::MessageType::_ERROR:
            textInfo.bold = true;
            textInfo.colorType = ColorType::RED;
            break;
        case Logger::MessageType::_CRITICAL:
            textInfo.bold = true;
            textInfo.colorType = ColorType::DEFAULT;
            textInfo.colorBgType = ColorBgType::RED;
            break;
    }
    setTextAttrib(textInfo);
    log(message);
    reset();
}

void Logger::log(std::string_view message) {
    time_t time = std::time(nullptr);
    std::string messageStr = {message.begin(), message.end()};
    printf("[%s] PANDA: %s", getTimeToString(time).c_str(), messageStr.c_str());
}

void Logger::reset() {
#ifdef PLATFORM_POSIX
    printf("\033[0m"); // reset
    printf("\n");
#elif defined(PLATFORM_WINDOWS)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\n");
#endif
}

void Logger::setTextAttrib(TextInfo textInfo) {
    ColorType colorType = textInfo.colorType;
    ColorBgType colorBgType = textInfo.colorBgType;
    bool bold = textInfo.bold;
#ifdef PLATFORM_POSIX
    // color
    const char *red = "\033[31m";
    const char *green = "\033[32m";
    const char *yellow = "\033[33m";
    const char *res = nullptr;
    switch (colorType) {
        case Logger::ColorType::DEFAULT:
            break;
        case Logger::ColorType::GREEN:
            res = green;
            break;
        case Logger::ColorType::YELLOW:
            res = yellow;
            break;
        case Logger::ColorType::RED:
            res = red;
            break;
    }
    if (res != nullptr) { printf("%s", res); }

    // bg
    res = nullptr;
    const char *redBg = "\033[41m";
    switch (colorBgType) {
        case Logger::ColorBgType::DEFAULT:
            break;
        case Logger::ColorBgType::RED:
            res = redBg;
            break;
    }
    if (res != nullptr) { printf("%s", res); }

    // bold
    if (bold) { printf("\033[1m"); }
#elif defined(PLATFORM_WINDOWS)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    auto attrib = FOREGROUND_INTENSITY;

    switch (colorType) {
        case Logger::ColorType::DEFAULT:
            break;
        case Logger::ColorType::GREEN:
            attrib |= FOREGROUND_GREEN;
            break;
        case Logger::ColorType::YELLOW:
            attrib |= FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        case Logger::ColorType::RED:
            attrib |= FOREGROUND_RED;
            break;
    }

    switch (colorBgType) {
        case Logger::ColorBgType::DEFAULT:
            break;
        case Logger::ColorBgType::RED:
            attrib |= BACKGROUND_RED;
            break;
    }

    SetConsoleTextAttribute(hConsole, attrib);
#endif
}

void EditorLogger::init(EditorLoggerCallback callback) {
    s_callback = callback;
}

void EditorLogger::log(std::string_view message, MessageType type) {
    s_callback(message, type);
}

} // namespace Foundation
