//
// Created by Admin on 11.02.2022.
//

#pragma once
#include "Panda/Renderer/GraphicsContext.hpp"

namespace Panda {

class GlesContext : public GraphicsContext {
    void create(int width, int height) override;
    void flip() override;
    void semaphoreWait() override;
    void semaphoreSignal() override;

private:
    void *context;
};

} // namespace Panda
