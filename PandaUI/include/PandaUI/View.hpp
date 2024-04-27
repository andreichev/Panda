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
    /* ------------------------------ Life cycle ------------------------------ */
    virtual void viewWillCalculateLayout() {}

    /* ------------------------------ Hierarchy ------------------------------ */
    void addSubview(Foundation::Shared<View> view);
    void removeSubview(Foundation::Shared<View> view);

    /* ------------------------------ Style ------------------------------ */
    void styleReset();
    void styleSetSize(Size size);
    void styleSetSizePercent(Size size);
    void styleSetOrigin(Point point);
    void styleSetOriginPercent(Point point);
    void styleSetMargins(EdgeInsets margins);
    void styleSetRelative();
    void styleSetAbsolute();
    void styleSetInsetsFromParent(EdgeInsets insets);
    void setTransform(const glm::mat4 &transform) {
        m_transform = transform;
    }
    void resetTransform() {
        m_transform = glm::mat4(1.f);
    }
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

    // TODO: - Добавить superview
protected:
    std::vector<Foundation::Shared<View>> m_subviews;
    Color m_backgroundColor;

private:
    void resolveHierarchy();
    void notifyWillCalculateLayout();
    void calculateLayout();
    void applyCalculatedLayout();
    Rect m_frame;
    uint8_t m_internal[20];
    glm::mat4 m_transform;
    void render(float offsetX, float offsetY);
    friend class Context;
};

} // namespace PandaUI