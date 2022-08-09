#include "UICrosshair.hpp"

UICrosshair::UICrosshair()
    : horizontal(Panda::makeShared<Panda::UIView>())
    , vertical(Panda::makeShared<Panda::UIView>()) {
    addSubviews();
}

void UICrosshair::addSubviews() {
    addSubview(horizontal);
    addSubview(vertical);
}

void UICrosshair::layout() {
    horizontal->setFrame(Panda::FRect(windowSize.width / 2 - 25, windowSize.height / 2 - 2, 50, 4));
    vertical->setFrame(Panda::FRect(windowSize.width / 2 - 2, windowSize.height / 2 - 25, 4, 50));
}