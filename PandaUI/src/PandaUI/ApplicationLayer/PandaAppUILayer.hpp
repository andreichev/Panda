//
// Created by Michael Andreichev on 28.10.2023.
//

#pragma once

#include "PandaUI/UIView.hpp"

#include <Panda/Application/Layer.hpp>
#include <Panda/Renderer/Renderer2D.hpp>

namespace PandaUI {

class PandaAppUILayer final : public Panda::Layer {
public:
    PandaAppUILayer();
    ~PandaAppUILayer() override = default;
    void onAttach() override;
    void onDetach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Panda::Event *event) override;

private:
    std::vector<UIView> m_views;
    Panda::Renderer2D m_renderer2d;
};

} // namespace PandaUI
