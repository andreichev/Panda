#include "Panda/ScriptRegistry.hpp"

namespace Panda {

ScriptRegistry *getScriptRegistry() {
    static ScriptRegistry globalData;
    return &globalData;
}

} // namespace Panda