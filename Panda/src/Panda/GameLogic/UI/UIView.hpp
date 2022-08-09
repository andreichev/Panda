//
// Created by Admin on 17.06.2022.
//

#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/Base/Rect.hpp"
#include "Panda/Events/WindowSizeListener.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Renderer/Miren.hpp"

namespace Panda {

class UIView : public WindowSizeListener {
public:
    UIView();
    UIView(FRect frame);
    virtual ~UIView();
    void render();
    void addSubview(Shared<UIView> node);
    void removeSubview(Shared<UIView> node);
    void setFrame(FRect frame);

    // Вызывается для позиционирования (обновление буфера)
    virtual void layout();
    // Вызывается для drawCall. Batching пока не настроен
    virtual void draw();

    // MARK: - WindowSizeListener

    void windowSizeChanged(UISize size) override;

    // TODO: - Добавить superview
protected:
    UISize windowSize;
    Window *window;

private:
    std::vector<Shared<UIView>> subviews;

    // TODO: - Один шейдер на все вьюхи
    ShaderHandle shaderHandle;
    VertexBufferHandle vertexBufferHandle;
    FRect frame;
};

} // namespace Panda