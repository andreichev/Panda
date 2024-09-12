#include "Logger.hpp"
#include "OuterScriptHook.hpp"

void Logger::log(std::string message) {
    Panda::ExternalCalls::consoleLog(message.c_str());
}