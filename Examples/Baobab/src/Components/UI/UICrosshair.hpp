#pragma once

#include <PandaUI/View.hpp>

class UICrosshair : public PandaUI::View {
public:
    UICrosshair();
    void layout() override;

private:
    Foundation::Shared<PandaUI::View> horizontal;
    Foundation::Shared<PandaUI::View> vertical;
    void addSubviews();
};