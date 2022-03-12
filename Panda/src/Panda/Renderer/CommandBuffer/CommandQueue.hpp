//
// Created by Admin on 05.03.2022.
//

#pragma once

#include "RendererCommand.hpp"

namespace Panda {

class CommandQueue {
public:
    CommandQueue();
    ~CommandQueue();
    void release(const RendererCommand * command);
    void post(RendererCommand * command);
    const RendererCommand * poll();
private:
    std::queue<RendererCommand *> commands;
};

}