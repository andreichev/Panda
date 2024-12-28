#pragma once

#include "WorldCommand.hpp"

#include <Foundation/Foundation.hpp>

namespace Panda {

class WorldCommandManager {
    inline static int TOTAL_OPERATIONS_IN_CACHE = 20;
    inline static int WORLD_COMMAND_SIZE = 256;
    inline static int WORLD_COMMAND_MANAGER_SIZE =
        WORLD_COMMAND_SIZE * (TOTAL_OPERATIONS_IN_CACHE * 2 + 5);

public:
    WorldCommandManager()
        : m_index(0)
        , m_undoCount(0)
        , m_redoCount(0)
        , m_data(nullptr) {
        m_data = (uint8_t *)F_ALLOC(Foundation::getAllocator(), WORLD_COMMAND_MANAGER_SIZE);
    }

    ~WorldCommandManager() {
        F_FREE(Foundation::getAllocator(), m_data);
    }

    /// Save new command to history and execute if it needs to.
    template<typename CMD>
    void SAVE(CMD cmd, bool needToExecute = true) {
        static_assert(std::is_base_of_v<WorldCommand, CMD>, "Not inherited from WorldCommand");
        PND_ASSERT_F(
            sizeof(CMD) < WORLD_COMMAND_SIZE,
            "NEED TO INCREASE COMMAND SIZE. REQUIRED {}",
            sizeof(CMD)
        );
        WorldCommand *last = PREVIOUS_COMMAND();
        if (last && last->canMerge(cmd)) {
            last->merge(cmd);
            cmd.isValid = needToExecute ? cmd.execute() : true;
            return;
        }
        cmd.isValid = needToExecute ? cmd.execute() : true;
        write(&cmd, sizeof(CMD), m_index);
        m_index++;
        m_index %= TOTAL_OPERATIONS_IN_CACHE;
        m_undoCount++;
        m_undoCount = Foundation::min(m_undoCount, TOTAL_OPERATIONS_IN_CACHE);
        m_redoCount = 0;
    }

    bool CAN_UNDO() {
        if (m_undoCount <= 0) {
            return false;
        }
        int prevIndex = (m_index + TOTAL_OPERATIONS_IN_CACHE - 1) % TOTAL_OPERATIONS_IN_CACHE;
        WorldCommand *prevCmd = (WorldCommand *)read(prevIndex);
        return prevCmd->isValid;
    }

    void UNDO() {
        if (m_undoCount <= 0) {
            return;
        }
        int prevIndex = (m_index + TOTAL_OPERATIONS_IN_CACHE - 1) % TOTAL_OPERATIONS_IN_CACHE;
        WorldCommand *prevCmd = (WorldCommand *)read(prevIndex);
        if (prevCmd->isValid && prevCmd->undo()) {
            m_undoCount--;
            m_undoCount = Foundation::max(0, m_undoCount);
            m_index = prevIndex;
            m_redoCount++;
            m_redoCount = Foundation::min(m_redoCount, TOTAL_OPERATIONS_IN_CACHE);
        }
    }

    bool CAN_REDO() {
        if (m_redoCount <= 0) {
            return false;
        }
        WorldCommand *cmd = (WorldCommand *)read(m_index);
        return cmd->isValid;
    }

    void REDO() {
        if (m_redoCount <= 0) {
            return;
        }
        WorldCommand *cmd = (WorldCommand *)read(m_index);
        if (cmd->isValid && cmd->execute()) {
            m_undoCount++;
            m_undoCount = Foundation::min(m_undoCount, TOTAL_OPERATIONS_IN_CACHE);
            m_index++;
            m_index %= TOTAL_OPERATIONS_IN_CACHE;
            m_redoCount--;
            m_redoCount = Foundation::max(0, m_redoCount);
        } else {
            cmd->isValid = false;
        }
    }

    WorldCommand *PREVIOUS_COMMAND() {
        if (m_undoCount <= 0) {
            return nullptr;
        }
        int prevIndex = (m_index + TOTAL_OPERATIONS_IN_CACHE - 1) % TOTAL_OPERATIONS_IN_CACHE;
        WorldCommand *cmd = (WorldCommand *)read(prevIndex);
        if (cmd->isValid) {
            return cmd;
        }
        return nullptr;
    }

    void CLEAR() {
        m_index = 0;
        m_undoCount = 0;
        m_redoCount = 0;
    }

private:
    void write(const void *_data, uint32_t _size, int index) {
        uint32_t pos = index * WORLD_COMMAND_SIZE;
        memcpy(&m_data[pos], _data, _size);
    }

    void *read(uint32_t index) {
        uint32_t pos = index * WORLD_COMMAND_SIZE;
        return &m_data[pos];
    }

    uint8_t *m_data;
    int m_index;
    int m_undoCount;
    int m_redoCount;
};

} // namespace Panda