#include "Bamboo/Input.hpp"
#include "Panda/OuterScriptHook.hpp"

namespace Bamboo {

bool Input::isKeyPressed(Key key) {
    return Panda::ExternalCalls::input_IsKeyPressed((int)key);
}

bool Input::isKeyJustPressed(Key key) {
    return Panda::ExternalCalls::input_IsKeyJustPressed((int)key);
}

} // namespace Bamboo