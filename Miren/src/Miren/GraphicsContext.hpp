//
// Created by Admin on 18.02.2022.
//

#pragma once

namespace Miren {
class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;
    virtual void create() = 0;
    virtual void flip() = 0;
};
} // namespace Miren
