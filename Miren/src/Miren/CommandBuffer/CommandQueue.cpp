//
// Created by Admin on 05.03.2022.
//

#include "CommandQueue.hpp"

namespace Miren {

CommandQueue::CommandQueue()
    : commands() {}

CommandQueue::~CommandQueue() {
    for (const RendererCommand *command = poll(); command != nullptr; command = poll()) {
        release(command);
    }
}

void CommandQueue::release(const RendererCommand *command) {
    delete command;
}

void CommandQueue::post(RendererCommand *command) {
    commands.push(command);
}

const RendererCommand *CommandQueue::poll() {
    if (commands.empty()) {
        return nullptr;
    }
    RendererCommand *command = commands.front();
    commands.pop();
    return command;
}

} // namespace Miren