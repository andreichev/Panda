//
// Created by Admin on 06/04/2022.
//

#pragma once

#include <Panda.hpp>

class CubeLayer : public Panda::Layer {
public:
    CubeLayer(Fern::Window *window);
    ~CubeLayer();

    void onAttach() override;
    void onDetach() override;

    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Fern::Event *event) override;

private:
    Fern::Window *m_window;
    glm::vec3 translate;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 projection;
    glm::mat4 viewProjection;

    double time;
    Miren::TextureHandle texture;
    Miren::ProgramHandle shader;
    Miren::IndexBufferHandle indexBuffer;
    Miren::VertexBufferHandle vertexBuffer;
};
