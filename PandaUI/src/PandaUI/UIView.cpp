#include "PandaUI/UIView.hpp"

namespace PandaUI {

UIView::UIView()
    : UIView(Rect::zero()) {}

UIView::~UIView() {}

UIView::UIView(Rect frame)
    : m_frame(frame)
    , m_backgroundColor(1.f, 1.f, 1.f, 1.f) {}

void UIView::setFrame(Rect frame) {
    m_frame = frame;
    layout();
}

void UIView::addSubview(Foundation::Shared<UIView> node) {
    m_subviews.push_back(node);
    node->layout();
}

void UIView::removeSubview(Foundation::Shared<UIView> node) {
    m_subviews.erase(find(m_subviews.begin(), m_subviews.end(), node));
}

void UIView::layout() {}

void UIView::draw() {}

void UIView::render() {
    draw();
    for (auto node : m_subviews) {
        node->render();
    }
}

} // namespace PandaUI