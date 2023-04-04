//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Panda/GameLogic/Entity.hpp"
#include "Panda/GameLogic/Components/OrthographicCamera.hpp"
#include "Panda/Window/Window.hpp"
#include "Panda/GameLogic/UI/UIView.hpp"

namespace Panda {

class World final {
public:
    World();
    void initialize();
    void update(double deltaTime);
    void onImGuiRender();
    Foundation::Shared<Entity> instantiateEntity();
    void destroy(Foundation::Shared<Entity> &entity);
    UIView *getUIView();
    void setOrthographicCamera(Foundation::Shared<OrthographicCamera> camera);
    inline Foundation::Shared<OrthographicCamera> getOrthographicCamera() const {
        return m_orthographicCamera;
    }

private:
    Entity root;
    UIView uiRoot;
    Foundation::Shared<OrthographicCamera> m_orthographicCamera;
};

} // namespace Panda