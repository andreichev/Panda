//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda.hpp>

class CameraMove : public Panda::Component {
public:
    void initialize() override;
    void update(double deltaTime) override;

private:
    float mouseSpeed = 0.1f;
    float moveSpeed = 20.0f;

    Foundation::Shared<Panda::Transform> transform;
    Panda::Window *window;
};
