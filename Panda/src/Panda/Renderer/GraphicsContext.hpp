//
// Created by Admin on 18.02.2022.
//

#pragma once

namespace Panda {
    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;
        virtual void create(int width, int height) = 0;
        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
    };
}
