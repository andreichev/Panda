//
// Created by Admin on 19/03/2022.
//

#pragma once

#include <Panda.hpp>

class ExampleLayer : public Panda::Layer {
public:
    ExampleLayer();
    void onAttach() override;
    void onDetach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Fern::Event *event) override;
};
