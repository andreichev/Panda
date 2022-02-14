//
// Created by Admin on 03.02.2022.
//

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Panda {

class Logger {
public:
    static void init();
    inline static std::shared_ptr<spdlog::logger> &getLogger() {
        return s_logger;
    };

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

} // namespace Panda

#define PND_TRACE(...) ::Panda::Logger::getLogger()->trace(__VA_ARGS__)
#define PND_INFO(...) ::Panda::Logger::getLogger()->info(__VA_ARGS__)
#define PND_WARN(...) ::Panda::Logger::getLogger()->warn(__VA_ARGS__)
#define PND_ERROR(...) ::Panda::Logger::getLogger()->error(__VA_ARGS__)
#define PND_CRITICAL(...) ::Panda::Logger::getLogger()->critical(__VA_ARGS__)