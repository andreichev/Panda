#include "Panda/Input.hpp"

#include "OuterScriptHook.hpp"

namespace Panda {

bool Input::isKeyPressed(Key key) {
    return ExternalCalls::input_IsKeyPressed((int)key);
}

bool Input::isKeyJustPressed(Key key) {
    return ExternalCalls::input_IsKeyJustPressed((int)key);
}

} // namespace Panda