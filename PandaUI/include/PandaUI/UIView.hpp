//
// Created by Admin on 17.06.2022.
//

#pragma once

#include "PandaUI/Base.hpp"

#include <Foundation/Foundation.hpp>

namespace PandaUI {

class UIView {
public:
    UIView();
    UIView(Rect frame);
    virtual ~UIView();
    void addSubview(Foundation::Shared<UIView> node);
    void removeSubview(Foundation::Shared<UIView> node);
    void setFrame(Rect frame);
    void setBackgroundColor(Color color) {
        m_backgroundColor = color;
    }
    Color getBackgroundColor() {
        return m_backgroundColor;
    }

    // Вызывается для позиционирования (обновление буфера)
    virtual void layout();
    // Вызывается для отрисовки.
    virtual void draw();

    // TODO: - Добавить superview
protected:
    std::vector<Foundation::Shared<UIView>> m_subviews;
    Rect m_frame;
    Color m_backgroundColor;

private:
    void render();
};

} // namespace PandaUI