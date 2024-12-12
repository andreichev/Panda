#include "SampleScript.hpp"

#include <Bamboo/Input.hpp>
#include <Bamboo/Logger.hpp>

using namespace Bamboo;

void SampleScript::update(float dt) {
    if (Input::isKeyPressed(Key::L)) {
        LOG("Hello Panda! var: {}", var);
    }
}