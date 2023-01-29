#pragma once

#include <Panda/GameLogic/UI/UIView.hpp>

class UICrosshair : public Panda::UIView {
public:
    UICrosshair();
    void layout() override;

private:
    Foundation::Shared<Panda::UIView> horizontal;
    Foundation::Shared<Panda::UIView> vertical;
    void addSubviews();
};