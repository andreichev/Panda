#pragma once

#include "Panels/Common/ImGuiHelper.hpp"
#include "EditorLayer/SceneState.hpp"

#include <Panda/ImGui/FontAwesome.h>

namespace Panda {

class ToolbarOutput {
public:
    virtual ~ToolbarOutput() = default;
    virtual void toolbarDidPickSceneState(SceneState state) = 0;
    virtual void toolbarDidTapReloadScripts() = 0;
    virtual void toolbarUndo() = 0;
    virtual void toolbarRedo() = 0;
    virtual SceneState toolbarGetCurrentSceneState() = 0;
};

class Toolbar final {
public:
    Toolbar(ToolbarOutput *output);
    void onImGuiRender(float offsetY);
    float getHeight() {
        return m_height;
    }

private:
    float m_height = 30;
    ToolbarOutput *m_output;
};

} // namespace Panda