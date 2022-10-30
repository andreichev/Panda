//
// Created by Admin on 03.02.2022.
//

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Foundation {

class Logger {
public:
    static void init();
    inline static std::shared_ptr<spdlog::logger> &getLogger() {
        return s_logger;
    };

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

} // namespace Foundation

#define PND_TRACE(...) ::Foundation::Logger::getLogger()->trace(__VA_ARGS__)
#define PND_INFO(...) ::Foundation::Logger::getLogger()->info(__VA_ARGS__)
#define PND_WARN(...) ::Foundation::Logger::getLogger()->warn(__VA_ARGS__)
#define PND_ERROR(...) ::Foundation::Logger::getLogger()->error(__VA_ARGS__)
#define PND_CRITICAL(...) ::Foundation::Logger::getLogger()->critical(__VA_ARGS__)