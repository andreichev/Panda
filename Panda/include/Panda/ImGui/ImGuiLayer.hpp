#pragma once

#include "Panda/Application/Layer.hpp"

#include <Fern/Window/Window.hpp>

namespace Panda {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer(Fern::Window *mainWindow);
    ~ImGuiLayer() = default;

    void onAttach() override;
    void onDetach() override;
    void onEvent(Fern::Event *event) override;

    void setBlockEvents(bool block) {
        m_blockEvents = block;
    }

    void begin(double deltaTime);
    void end();

    void setDarkThemeColors();

private:
    Fern::Window *m_window;
    bool m_blockEvents = true;
};

} // namespace Panda
