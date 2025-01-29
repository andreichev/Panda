#pragma once

#include "WorldCommand.hpp"

#include <Foundation/Foundation.hpp>

namespace Panda {

class WorldCommandManager final {
    inline static int MAX_OPERATIONS_IN_CACHE = 1;

public:
    WorldCommandManager()
        : undoStack(MAX_OPERATIONS_IN_CACHE)
        , redoStack(MAX_OPERATIONS_IN_CACHE) {}

    /// Save new command to history and execute if it needs to.
    template<typename CMD>
    void SAVE(CMD &cmd, bool needToExecute = true) {
        static_assert(std::is_base_of_v<WorldCommand, CMD>, "Not inherited from WorldCommand");
        auto last = PREVIOUS_COMMAND();
        if (last && last->canMerge(cmd)) {
            last->merge(cmd);
        } else {
            undoStack.push_back(Foundation::makeShared<CMD>(std::move(cmd)));
        }
        if (needToExecute) {
            last = PREVIOUS_COMMAND();
            if (!last->execute()) {
                LOG_ERROR("ERROR EXECUTING COMMAND");
                CLEAR();
            }
        }
        if (undoStack.size() > MAX_OPERATIONS_IN_CACHE) {
            undoStack.pop_front();
        }
        redoStack.clear();
    }

    bool CAN_UNDO() {
        return !undoStack.empty();
    }

    void UNDO() {
        if (!CAN_UNDO()) {
            return;
        }
        auto command = undoStack.back();
        undoStack.pop_back();
        if (!command->undo()) {
            LOG_ERROR("ERROR EXECUTING COMMAND");
            CLEAR();
            return;
        }
        redoStack.push_back(command);
    }

    bool CAN_REDO() {
        return !redoStack.empty();
    }

    void REDO() {
        if (!CAN_REDO()) {
            return;
        }
        auto command = redoStack.back();
        redoStack.pop_back();
        if (!command->execute()) {
            LOG_ERROR("ERROR EXECUTING COMMAND");
            CLEAR();
            return;
        }
        undoStack.push_back(command);
    }

    Foundation::Shared<WorldCommand> PREVIOUS_COMMAND() {
        if (undoStack.empty()) {
            return nullptr;
        }
        return undoStack.back();
    }

    void CLEAR() {
        undoStack.clear();
        redoStack.clear();
    }

private:
    std::deque<Foundation::Shared<WorldCommand>> undoStack;
    std::deque<Foundation::Shared<WorldCommand>> redoStack;
};

} // namespace Panda