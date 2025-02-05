#include <imgui.h>

namespace ImGui {

bool TreeNodeCustom(
    ImGuiID id,
    ImGuiTreeNodeFlags flags,
    const char *label,
    const char *label_end,
    bool *out_pressed
);

}