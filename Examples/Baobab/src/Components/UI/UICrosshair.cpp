#include "UICrosshair.hpp"

UICrosshair::UICrosshair()
    : horizontal(PandaUI::makeView<PandaUI::View>())
    , vertical(PandaUI::makeView<PandaUI::View>()) {
    addSubviews();
}

UICrosshair::~UICrosshair() {
    PandaUI::freeView(horizontal);
    PandaUI::freeView(vertical);
}

void UICrosshair::addSubviews() {
    addSubview(horizontal);
    addSubview(vertical);
}

void UICrosshair::layout() {
    // horizontal->setFrame(
    //     PandaUI::Rect(m_frame.size.width / 2 - 25, m_frame.size.height / 2 - 2, 50, 4));
    // vertical->setFrame(
    //     PandaUI::Rect(m_frame.size.width / 2 - 2, m_frame.size.height / 2 - 25, 4, 50));
    Panda::Size windowSize = Panda::Application::get()->getWindow()->getSize();
    horizontal->setFrame(PandaUI::Rect(windowSize.width / 2 - 25, windowSize.height / 2 - 2, 50, 4)
    );
    vertical->setFrame(PandaUI::Rect(windowSize.width / 2 - 2, windowSize.height / 2 - 25, 4, 50));
}
