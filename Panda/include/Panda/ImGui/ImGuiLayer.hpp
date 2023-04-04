#pragma once

#include "Panda/Application/Layer.hpp"

namespace Panda {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    void onAttach() override;
    void onDetach() override;
    void onEvent(Event *event) override;

    void begin(double deltaTime);
    void end();

    void setDarkThemeColors();
};

} // namespace Panda