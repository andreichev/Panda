#pragma once

#include <PandaScriptHook.hpp>

namespace Panda {

void initScriptGlue();
void *loadScriptFunc(const char *name);

} // namespace Panda