#pragma once

#include <Bamboo/Script.hpp>
#include <Bamboo/Bamboo.hpp>

class SampleScript : public Bamboo::Script {
public:
    int var;

    PANDA_FIELDS_BEGIN(SampleScript)
    PANDA_FIELD(var)
    PANDA_FIELDS_END

    void update(float dt) override;
};

REGISTER_SCRIPT(SampleScript)
