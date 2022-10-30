//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Miren/GraphicsContext.hpp"

namespace Miren {

class GlesContext : public GraphicsContext {
    void create() override;
    void flip() override;

private:
    void *context;
};

} // namespace Miren
