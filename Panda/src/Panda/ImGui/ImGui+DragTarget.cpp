#include "Panda/ImGui/ImGui+DragTarget.hpp"

#include <imgui_internal.h>

namespace ImGui {

bool BeginDragDropTargetCustom(bool allowToDropToItself) {
    ImGuiContext &g = *GImGui;
    if (!g.DragDropActive) return false;

    ImGuiWindow *window = g.CurrentWindow;
    if (!(g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect)) return false;
    ImGuiWindow *hovered_window = g.HoveredWindowUnderMovingWindow;
    if (hovered_window == NULL ||
        window->RootWindowDockTree != hovered_window->RootWindowDockTree || window->SkipItems)
        return false;

    const ImRect &display_rect = (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HasDisplayRect)
                                     ? g.LastItemData.DisplayRect
                                     : g.LastItemData.Rect;
    ImGuiID id = g.LastItemData.ID;
    if (id == 0) {
        id = window->GetIDFromRectangle(display_rect);
        KeepAliveID(id);
    }
    if (g.DragDropPayload.SourceId == id && !allowToDropToItself) return false;

    IM_ASSERT(
        g.DragDropWithinTarget == false && g.DragDropWithinSource == false
    ); // Can't nest BeginDragDropSource() and BeginDragDropTarget()
    g.DragDropTargetRect = display_rect;
    g.DragDropTargetClipRect = (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HasClipRect)
                                   ? g.LastItemData.ClipRect
                                   : window->ClipRect;
    g.DragDropTargetId = id;
    g.DragDropWithinTarget = true;
    return true;
}

} // namespace ImGui