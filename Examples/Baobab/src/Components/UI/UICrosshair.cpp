#include "UICrosshair.hpp"

UICrosshair::UICrosshair()
    : horizontal(PandaUI::makeView<PandaUI::View>())
    , vertical(PandaUI::makeView<PandaUI::View>()) {
    setupStyle();
    addSubviews();
    makeConstraints();
}

void UICrosshair::setupStyle() {
    m_backgroundColor = 0xFFFFFF00;
}

void UICrosshair::addSubviews() {
    addSubview(horizontal);
    addSubview(vertical);
}

void UICrosshair::makeConstraints() {
    horizontal->styleSetAbsolute();
    horizontal->styleSetOriginPercent({50.f, 50.f});
    horizontal->setTransform(glm::translate(glm::mat4(1.f), {-25, -2.0, 0.0}));
    horizontal->styleSetSize({50, 4});
    vertical->styleSetAbsolute();
    vertical->styleSetOriginPercent({50.f, 50.f});
    vertical->setTransform(glm::translate(glm::mat4(1.f), {-2.0, -25, 0.0}));
    vertical->styleSetSize({4, 50});
}
