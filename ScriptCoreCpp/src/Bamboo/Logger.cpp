#include "Bamboo/Logger.hpp"
#include "Panda/OuterScriptHook.hpp"

namespace Bamboo {

void Logger::log(std::string message) {
    Panda::ExternalCalls::console_Log(message.c_str());
}

} // namespace Bamboo