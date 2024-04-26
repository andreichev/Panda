#pragma once

#include "PandaUI/View.hpp"

class RootView final : public PandaUI::View {
public:
    RootView() {
        using namespace PandaUI;
        m_backgroundColor = 0x994411FF;

        Foundation::Shared<View> subview1 = Foundation::makeShared<View>();
        subview1->setBackgroundColor(0xFF5522FF);
        subview1->styleSetAbsolute();
        subview1->styleSetInsetsFromParent({20, 30, 30, 100});

        addSubview(subview1);

        Foundation::Shared<View> subview2 = Foundation::makeShared<View>();
        subview2->setBackgroundColor(0x11FF22FF);
        subview2->styleSetAbsolute();
        subview2->styleSetInsetsFromParent({30, 30, 30, 200});

        subview1->addSubview(subview2);
    }
};