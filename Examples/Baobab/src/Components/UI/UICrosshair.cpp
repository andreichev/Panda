#include "UICrosshair.hpp"

UICrosshair::UICrosshair()
    : horizontal(Foundation::makeShared<PandaUI::UIView>())
    , vertical(Foundation::makeShared<PandaUI::UIView>()) {
    addSubviews();
}

void UICrosshair::addSubviews() {
    addSubview(horizontal);
    addSubview(vertical);
}

void UICrosshair::layout() {
    horizontal->setFrame(
        PandaUI::Rect(m_frame.size.width / 2 - 25, m_frame.size.height / 2 - 2, 50, 4));
    vertical->setFrame(
        PandaUI::Rect(m_frame.size.width / 2 - 2, m_frame.size.height / 2 - 25, 4, 50));
}