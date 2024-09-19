#include "Logger.hpp"
#include "OuterScriptHook.hpp"

void Logger::log(std::string message) {
    Panda::ExternalCalls::console_Log(message.c_str());
}