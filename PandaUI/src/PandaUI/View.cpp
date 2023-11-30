#include "PandaUI/View.hpp"

namespace PandaUI {

View::View()
    : View(Rect::zero()) {}

View::~View() {}

View::View(Rect frame)
    : m_frame(frame)
    , m_backgroundColor(1.f, 1.f, 1.f, 1.f) {}

void View::setFrame(Rect frame) {
    m_frame = frame;
    layout();
}

void View::addSubview(View *node) {
    m_subviews.push_back(node);
    node->layout();
}

void View::removeSubview(View *node) {
    m_subviews.erase(find(m_subviews.begin(), m_subviews.end(), node));
}

void View::layout() {}

} // namespace PandaUI