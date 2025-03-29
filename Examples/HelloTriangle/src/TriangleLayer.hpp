//
// Created by Admin on 19/03/2022.
//

#pragma once

#include "Panda.hpp"

class TriangleLayer final : public Panda::Layer {
public:
    TriangleLayer();
    ~TriangleLayer();

    void onAttach() override;
    void onDetach() override;

    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Fern::Event *event) override;

private:
    Miren::VertexBufferHandle m_vertexBuffer;
    Miren::IndexBufferHandle m_indexBuffer;
    Miren::ProgramHandle m_shader;
};
