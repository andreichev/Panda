#pragma once

#include "UICrosshair.hpp"
#include <PandaUI/View.hpp>

class RootView final : public PandaUI::View {
public:
    RootView() {
        using namespace PandaUI;
        m_backgroundColor = 0xFFFFFF00;

        Foundation::Shared<UICrosshair> crosshair = makeView<UICrosshair>();
        addSubview(crosshair);
        crosshair->styleSetAbsolute();
        crosshair->styleSetInsetsFromParent({0, 0, 0, 0});
    }
};