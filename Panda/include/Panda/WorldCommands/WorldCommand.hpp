#pragma once

#include <cstdint>

namespace Panda {

class WorldCommand {
public:
    virtual ~WorldCommand() = default;
    virtual bool execute() = 0;
    virtual bool undo() = 0;
    virtual void merge(WorldCommand &command) {}
    virtual bool canMerge(WorldCommand &command) {
        return false;
    }
    // TODO: Store selection context for each command
};

} // namespace Panda