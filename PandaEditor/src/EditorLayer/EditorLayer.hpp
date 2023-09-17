#pragma once

#include "Panels/Viewport.hpp"
#include "Panels/WorldHierarchyPanel.hpp"

#include <Panda.hpp>

namespace Panda {

class EditorLayer : public Layer {
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    void onUpdate(double deltaTime) override;
    virtual void onImGuiRender() override;
    void onEvent(Event *event) override;

private:
    void initializeExampleWorld();

    Viewport m_viewport;
    WorldHierarchyPanel m_hierarchyPanel;
    World *m_world;
};

} // namespace Panda