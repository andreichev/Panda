#pragma once

namespace Panda {

using PopupActionFunction = void (*)(void *userData);

class EditorPopup {
public:
    virtual ~EditorPopup() = default;
    virtual void onImGuiRender() = 0;

    const char *title;
};

} // namespace Panda
