//
// Created by Admin on 11.02.2022.
//

#pragma once
#include "Panda/Renderer/GraphicsContext.hpp"

namespace Panda {

class GlesContext : public GraphicsContext {
    void create() override;
    void flip() override;

private:
    void *context;
};

} // namespace Panda
