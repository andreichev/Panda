#include "Bamboo/Logger.hpp"
#include "Panda/OuterScriptHook.hpp"

namespace Bamboo {

void Logger::log(MessageType type, std::string message) {
    Panda::ExternalCalls::console_Log(type, message.c_str());
}

} // namespace Bamboo