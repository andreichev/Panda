#pragma once

#include <ScriptCore.hpp>
#include <ScriptRegistry.hpp>

class SampleScript : public Panda::Script {
public:
    void update(float dt) override;
};

REGISTER_SCRIPT(SampleScript)
