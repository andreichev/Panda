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
    /* ------------------------------ Hierarchy ------------------------------ */
    void addSubview(Foundation::Shared<View> node);
    void removeSubview(Foundation::Shared<View> node);

    /* ------------------------------ Style ------------------------------ */
    void styleReset();
    void styleSetSize(Size size);
    void styleSetOrigin(Point point);
    void styleSetMargins(EdgeInsets margins);
    void styleSetRelative();
    void styleSetAbsolute();
    void styleSetInsetsFromParent(EdgeInsets insets);

    /* ------------------------------ Properties ------------------------------ */
    void setFrame(Rect frame) {
        m_frame = frame;
    }
    Rect getFrame() {
        return m_frame;
    }
    void setBackgroundColor(Color color) {
        m_backgroundColor = color;
    }
    const Color &getBackgroundColor() {
        return m_backgroundColor;
    }
    std::vector<Foundation::Shared<View>> getSubviews() {
        return m_subviews;
    }

    virtual void calculateLayout();

    // TODO: - Добавить superview
protected:
    std::vector<Foundation::Shared<View>> m_subviews;
    Color m_backgroundColor;

private:
    void applyCalculatedLayout();
    Rect m_frame;
    uint8_t m_internal[20];
    void render(float offsetX, float offsetY);
    friend class Context;
};

} // namespace PandaUI