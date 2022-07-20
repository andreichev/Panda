#pragma once

#include "Panda/Application/Layer.hpp"
#include "Panda/GameLogic/World.hpp"
#include "Panda/GameLogic/Level.hpp"

namespace Panda {

class BasicGameLayer : public Layer {
public:
    BasicGameLayer(Level *startupLevel);
    virtual ~BasicGameLayer() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    void onUpdate(double deltaTime) override;
    virtual void onImGuiRender() override;
    void onEvent(Event *event) override;

private:
    World *m_world;
    Level *m_currentLevel;
};

} // namespace Panda