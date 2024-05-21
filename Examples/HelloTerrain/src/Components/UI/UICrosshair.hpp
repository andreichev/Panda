#pragma once

#include <PandaUI/PandaUI.hpp>

class UICrosshair : public PandaUI::View {
public:
    UICrosshair();

private:
    void setupStyle();
    void addSubviews();
    void makeConstraints();

    Foundation::Shared<PandaUI::View> horizontal;
    Foundation::Shared<PandaUI::View> vertical;
};