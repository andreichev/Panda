//
// Created by Admin on 17.06.2022.
//

#pragma once

#include "Panda/Events/WindowSizeListener.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/Base/Base.hpp"

#include <Miren/Miren.hpp>
#include <Foundation/Foundation.hpp>

namespace Panda {

class UIView : public WindowSizeListener {
public:
    UIView();
    UIView(Rect frame);
    virtual ~UIView();
    void render();
    void addSubview(Foundation::Shared<UIView> node);
    void removeSubview(Foundation::Shared<UIView> node);
    void setFrame(Rect frame);

    // Вызывается для позиционирования (обновление буфера)
    virtual void layout();
    // Вызывается для drawCall. Batching пока не настроен
    virtual void draw();

    // MARK: - WindowSizeListener

    void windowSizeChanged(Size size) override;

    // TODO: - Добавить superview
protected:
    Size windowSize;
    Window *window;

private:
    std::vector<Foundation::Shared<UIView>> subviews;

    // TODO: - Один шейдер на все вьюхи
    Miren::ShaderHandle shaderHandle;
    Miren::VertexBufferHandle vertexBufferHandle;
    Rect frame;
};

} // namespace Panda