#pragma once

#include <PandaUI/UIView.hpp>

class UICrosshair : public PandaUI::UIView {
public:
    UICrosshair();
    void layout() override;

private:
    Foundation::Shared<PandaUI::UIView> horizontal;
    Foundation::Shared<PandaUI::UIView> vertical;
    void addSubviews();
};