//
// Created by Admin on 17.06.2022.
//

#pragma once

#include "PandaUI/Base.hpp"

#include <Foundation/Foundation.hpp>

namespace PandaUI {

class View {
public:
    View();
    View(Rect frame);
    virtual ~View();
    void addSubview(View *node);
    void removeSubview(View *node);
    void setFrame(Rect frame);
    const Rect &getFrame() const {
        return m_frame;
    }
    void setBackgroundColor(Color color) {
        m_backgroundColor = color;
    }
    const Color &getBackgroundColor() {
        return m_backgroundColor;
    }

    // Вызывается для позиционирования (обновление буфера)
    virtual void layout();

    // TODO: - Добавить superview
protected:
    std::vector<View *> m_subviews;
    Rect m_frame;
    Color m_backgroundColor;
};

} // namespace PandaUI