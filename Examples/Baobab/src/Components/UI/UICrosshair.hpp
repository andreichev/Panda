#pragma once

#include <PandaUI/PandaUI.hpp>

class UICrosshair : public PandaUI::View {
public:
    UICrosshair();
    ~UICrosshair() override;
    void layout() override;

private:
    PandaUI::View *horizontal;
    PandaUI::View *vertical;
    void addSubviews();
};