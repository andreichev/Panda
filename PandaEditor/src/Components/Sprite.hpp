//
// Created by Michael Andreichev on 17.09.2023.
//

#pragma once

#include <Panda/GameLogic/NativeScript.hpp>

namespace Panda {

class Sprite final : public NativeScript {
public:
    Sprite()
        : m_transform(NULL)
        , m_color(1.0f, 0.5f, 0.2f, 1.0f) {}

    void setColor(Color color) {
        m_color = color;
    }

private:
    void initialize() override;
    void update(double deltaTime) override;
    Transform *m_transform;
    Color m_color;
};

} // namespace Panda