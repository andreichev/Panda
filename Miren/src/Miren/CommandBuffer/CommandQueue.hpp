//
// Created by Admin on 05.03.2022.
//

#pragma once

#include "RendererCommand.hpp"

#include <queue>

namespace Miren {

class CommandQueue {
public:
    CommandQueue();
    ~CommandQueue();
    void release(const RendererCommand *command);
    void post(RendererCommand *command);
    const RendererCommand *poll();

private:
    std::queue<RendererCommand *> commands;
};

} // namespace Miren