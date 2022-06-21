//
// Created by Admin on 17.06.2022.
//

#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/Base/GRect.hpp"
#include "Panda/Events/WindowSizeListener.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Events/Input.hpp"
#include "Panda/Renderer/Miren.hpp"

namespace Panda {

class UIView : public WindowSizeListener {
public:
    UIView();
    UIView(GRect frame);
    virtual ~UIView();
    void render();
    void addSubview(Shared<UIView> node);
    void removeSubview(Shared<UIView> node);
    void setFrame(GRect frame);

    // Вызывается для позиционирования (обновление буфера)
    virtual void layout();
    // Вызывается для drawCall. Batching пока не настроен
    virtual void draw();

    // MARK: - WindowSizeListener

    void windowSizeChanged(GSize size) override;

    // TODO: - Добавить superview
protected:
    GSize windowSize;
    Window *window;
    Input *input;

private:
    std::vector<Shared<UIView>> subviews;

    // TODO: - Один шейдер на все вьюхи
    ShaderHandle shaderHandle;
    VertexBufferHandle vertexBufferHandle;
    GRect frame;
};

} // namespace Panda