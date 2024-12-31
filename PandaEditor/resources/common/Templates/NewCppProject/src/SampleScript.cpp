#include "SampleScript.hpp"

#include <Bamboo/Input.hpp>
#include <Bamboo/Logger.hpp>

void SampleScript::update(float dt) {
    if (Input::isKeyPressed(Key::L)) {
        LOG_INFO("Hello Panda! var: {}", var);
    }
}