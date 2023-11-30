//
// Created by Michael Andreichev on 28.10.2023.
//

#pragma once

#include "PandaUI/Context.hpp"

#include <Panda/Application/Layer.hpp>

namespace PandaUI {

class Layer final : public Panda::Layer {
public:
    Layer();
    ~Layer() override;
    void onAttach() override;
    void onDetach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Panda::Event *event) override;

private:
    Context m_context;
};

} // namespace PandaUI
