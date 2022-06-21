#pragma once

#include <Panda/GameLogic/UI/UIView.hpp>

class UICrosshair : public Panda::UIView {
public:
    UICrosshair();
    void layout() override;

private:
    Panda::Shared<Panda::UIView> horizontal;
    Panda::Shared<Panda::UIView> vertical;
    void addSubviews();
};