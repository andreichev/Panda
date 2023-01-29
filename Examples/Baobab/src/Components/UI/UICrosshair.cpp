#include "UICrosshair.hpp"

UICrosshair::UICrosshair()
    : horizontal(Foundation::makeShared<Panda::UIView>())
    , vertical(Foundation::makeShared<Panda::UIView>()) {
    addSubviews();
}

void UICrosshair::addSubviews() {
    addSubview(horizontal);
    addSubview(vertical);
}

void UICrosshair::layout() {
    horizontal->setFrame(Panda::Rect(windowSize.width / 2 - 25, windowSize.height / 2 - 2, 50, 4));
    vertical->setFrame(Panda::Rect(windowSize.width / 2 - 2, windowSize.height / 2 - 25, 4, 50));
}