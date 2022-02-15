//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Panda/Events/IEvents.hpp>
#include <Panda/GameLogic/Components/Transform.hpp>

class CameraMove : public Panda::Component {
public:
    void initialize() override;
    void update(float deltaTime) override;

private:
    float mouseSpeed = 0.1f;
    float moveSpeed = 20.0f;

    Panda::Shared<Panda::Transform> transform;
    Panda::IEvents *events;
};
