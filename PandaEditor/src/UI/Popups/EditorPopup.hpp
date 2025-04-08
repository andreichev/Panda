#pragma once

#include <functional>

namespace Panda {

using PopupActionFunction = std::function<void()>;

class EditorPopup {
public:
    virtual ~EditorPopup() = default;
    virtual void onImGuiRender() = 0;

    const char *title;
};

} // namespace Panda
