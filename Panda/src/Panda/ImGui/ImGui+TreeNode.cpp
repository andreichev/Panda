#include "Panda/ImGui/ImGui+TreeNode.hpp"

#include <imgui_internal.h>

namespace ImGui {

bool TreeNodeCustom(
    ImGuiID id,
    ImGuiTreeNodeFlags flags,
    const char *label,
    const char *label_end,
    bool *out_pressed
) {
    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    const ImVec2 padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding))
                               ? style.FramePadding
                               : ImVec2(
                                     style.FramePadding.x,
                                     ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y)
                                 );

    if (!label_end) label_end = FindRenderedTextEnd(label);
    const ImVec2 label_size = CalcTextSize(label, label_end, false);

    const float text_offset_x = g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2
                                             ); // Collapsing arrow width + Spacing
    const float text_offset_y =
        ImMax(padding.y, window->DC.CurrLineTextBaseOffset); // Latch before ItemSize changes it
    const float text_width = g.FontSize + label_size.x + padding.x * 2; // Include collapsing arrow

    // We vertically grow up to current line height up the typical widget height.
    const float frame_height = ImMax(
        ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y * 2),
        label_size.y + padding.y * 2
    );
    const bool span_all_columns =
        (flags & ImGuiTreeNodeFlags_SpanAllColumns) != 0 && (g.CurrentTable != NULL);
    ImRect frame_bb;
    frame_bb.Min.x = span_all_columns                             ? window->ParentWorkRect.Min.x
                     : (flags & ImGuiTreeNodeFlags_SpanFullWidth) ? window->WorkRect.Min.x
                                                                  : window->DC.CursorPos.x;
    frame_bb.Min.y = window->DC.CursorPos.y;
    frame_bb.Max.x = span_all_columns ? window->ParentWorkRect.Max.x
                     : (flags & ImGuiTreeNodeFlags_SpanTextWidth)
                         ? window->DC.CursorPos.x + text_width + padding.x
                         : window->WorkRect.Max.x;
    frame_bb.Max.y = window->DC.CursorPos.y + frame_height;
    if (display_frame) {
        const float outer_extend = IM_TRUNC(
            window->WindowPadding.x * 0.5f
        ); // Framed header expand a little outside of current limits
        frame_bb.Min.x -= outer_extend;
        frame_bb.Max.x += outer_extend;
    }

    ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
    ItemSize(ImVec2(text_width, frame_height), padding.y);

    // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
    ImRect interact_bb = frame_bb;
    if ((flags & (ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                  ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanTextWidth |
                  ImGuiTreeNodeFlags_SpanAllColumns)) == 0)
        interact_bb.Max.x =
            frame_bb.Min.x + text_width + (label_size.x > 0.0f ? style.ItemSpacing.x * 2.0f : 0.0f);

    // Compute open and multi-select states before ItemAdd() as it clear NextItem data.
    ImGuiID storage_id = (g.NextItemData.Flags & ImGuiNextItemDataFlags_HasStorageID)
                             ? g.NextItemData.StorageId
                             : id;
    bool is_open = TreeNodeUpdateNextOpen(storage_id, flags);

    bool is_visible;
    if (span_all_columns) {
        // Modify ClipRect for the ItemAdd(), faster than doing a
        // PushColumnsBackground/PushTableBackgroundChannel for every Selectable..
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        is_visible = ItemAdd(interact_bb, id);
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    } else {
        is_visible = ItemAdd(interact_bb, id);
    }
    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
    g.LastItemData.DisplayRect = frame_bb;

    // If a NavLeft request is happening and ImGuiTreeNodeFlags_NavLeftJumpsBackHere enabled:
    // Store data for the current depth to allow returning to this node from any child item.
    // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode()
    // and TreePop(). It will become tempting to enable ImGuiTreeNodeFlags_NavLeftJumpsBackHere by
    // default or move it to ImGuiStyle.
    bool store_tree_node_stack_data = false;
    if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
        if ((flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) && is_open && !g.NavIdIsAlive)
            if (g.NavMoveDir == ImGuiDir_Left && g.NavWindow == window &&
                NavMoveRequestButNoResultYet())
                store_tree_node_stack_data = true;
    }

    const bool is_leaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
    if (!is_visible) {
        // if (store_tree_node_stack_data && is_open)
        //     TreeNodeStoreStackData(flags); // Call before TreePushOverrideID()
        if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen)) TreePushOverrideID(id);
        IMGUI_TEST_ENGINE_ITEM_INFO(
            g.LastItemData.ID,
            label,
            g.LastItemData.StatusFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) |
                (is_open ? ImGuiItemStatusFlags_Opened : 0)
        );
        return is_open;
    }

    if (span_all_columns) {
        TablePushBackgroundChannel();
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
        g.LastItemData.ClipRect = window->ClipRect;
    }

    ImGuiButtonFlags button_flags = ImGuiTreeNodeFlags_None;
    if ((flags & ImGuiTreeNodeFlags_AllowOverlap) ||
        (g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap))
        button_flags |= ImGuiButtonFlags_AllowOverlap;
    if (!is_leaf) button_flags |= ImGuiButtonFlags_PressedOnDragDropHold;

    // We allow clicking on the arrow section with keyboard modifiers held, in order to easily
    // allow browsing a tree while preserving selection with code implementing multi-selection
    // patterns. When clicking on the rest of the tree node we always disallow keyboard modifiers.
    const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
    const float arrow_hit_x2 =
        (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
    const bool is_mouse_x_over_arrow =
        (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);

    const bool is_multi_select = (g.LastItemData.InFlags & ImGuiItemFlags_IsMultiSelect) != 0;
    if (is_multi_select) // We absolutely need to distinguish open vs select so _OpenOnArrow comes
                         // by default
        flags |= (flags & ImGuiTreeNodeFlags_OpenOnMask_) == 0
                     ? ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
                     : ImGuiTreeNodeFlags_OpenOnArrow;

    button_flags |= ImGuiButtonFlags_PressedOnClickRelease;

    bool selected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
    const bool was_selected = selected;

    // Multi-selection support (header)
    if (is_multi_select) {
        // Handle multi-select + alter button flags for it
        MultiSelectItemHeader(id, &selected, &button_flags);
        if (is_mouse_x_over_arrow)
            button_flags = (button_flags | ImGuiButtonFlags_PressedOnClick) &
                           ~ImGuiButtonFlags_PressedOnClickRelease;
    }
    // else {
    //     if (window != g.HoveredWindow || !is_mouse_x_over_arrow)
    //         button_flags |= ImGuiButtonFlags_NoKeyModifiers;
    // }

    bool hovered, held;
    bool pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    *out_pressed = pressed;

    bool toggled = false;
    if (!is_leaf) {
        if (pressed && g.DragDropHoldJustPressedId != id) {
            if ((flags & ImGuiTreeNodeFlags_OpenOnMask_) == 0 ||
                (g.NavActivateId == id && !is_multi_select))
                toggled = true; // Single click
            if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
                toggled |=
                    is_mouse_x_over_arrow &&
                    !g.NavDisableMouseHover; // Lightweight equivalent of IsMouseHoveringRect()
                                             // since ButtonBehavior() already did the job
            if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseClickedCount[0] == 2)
                toggled = true; // Double click
        } else if (pressed && g.DragDropHoldJustPressedId == id) {
            IM_ASSERT(button_flags & ImGuiButtonFlags_PressedOnDragDropHold);
            if (!is_open) // When using Drag and Drop "hold to open" we keep the node highlighted
                          // after opening, but never close it again.
                toggled = true;
            else
                pressed = false; // Cancel press so it doesn't trigger selection.
        }

        if (g.NavId == id && g.NavMoveDir == ImGuiDir_Left && is_open) {
            toggled = true;
            NavClearPreferredPosForAxis(ImGuiAxis_X);
            NavMoveRequestCancel();
        }
        if (g.NavId == id && g.NavMoveDir == ImGuiDir_Right &&
            !is_open) // If there's something upcoming on the line we may want to give it the
                      // priority?
        {
            toggled = true;
            NavClearPreferredPosForAxis(ImGuiAxis_X);
            NavMoveRequestCancel();
        }

        if (toggled) {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(storage_id, is_open);
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
        }
    }

    // Multi-selection support (footer)
    if (is_multi_select) {
        bool pressed_copy = pressed && !toggled;
        MultiSelectItemFooter(id, &selected, &pressed_copy);
        if (pressed) SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, interact_bb);
    }

    if (selected != was_selected)
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    // Render
    {
        const ImU32 text_col = GetColorU32(ImGuiCol_Text);
        ImGuiNavHighlightFlags nav_highlight_flags = ImGuiNavHighlightFlags_Compact;
        if (is_multi_select)
            nav_highlight_flags |=
                ImGuiNavHighlightFlags_AlwaysDraw; // Always show the nav rectangle
        if (display_frame) {
            // Framed type
            const ImU32 bg_col = GetColorU32(
                (held && hovered) ? ImGuiCol_HeaderActive
                : hovered         ? ImGuiCol_HeaderHovered
                                  : ImGuiCol_Header
            );
            RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, true, style.FrameRounding);
            RenderNavHighlight(frame_bb, id, nav_highlight_flags);
            if (flags & ImGuiTreeNodeFlags_Bullet)
                RenderBullet(
                    window->DrawList,
                    ImVec2(text_pos.x - text_offset_x * 0.60f, text_pos.y + g.FontSize * 0.5f),
                    text_col
                );
            else if (!is_leaf)
                RenderArrow(
                    window->DrawList,
                    ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y),
                    text_col,
                    is_open ? ((flags & ImGuiTreeNodeFlags_UpsideDownArrow) ? ImGuiDir_Up
                                                                            : ImGuiDir_Down)
                            : ImGuiDir_Right,
                    1.0f
                );
            else // Leaf without bullet, left-adjusted text
                text_pos.x -= text_offset_x - padding.x;
            if (flags & ImGuiTreeNodeFlags_ClipLabelForTrailingButton)
                frame_bb.Max.x -= g.FontSize + style.FramePadding.x;
            if (g.LogEnabled) LogSetNextTextDecoration("###", "###");
        } else {
            // Unframed typed for tree nodes
            if (hovered || selected) {
                const ImU32 bg_col = GetColorU32(
                    (held && hovered) ? ImGuiCol_HeaderActive
                    : hovered         ? ImGuiCol_HeaderHovered
                                      : ImGuiCol_Header
                );
                RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, false);
            }
            RenderNavHighlight(frame_bb, id, nav_highlight_flags);
            if (flags & ImGuiTreeNodeFlags_Bullet)
                RenderBullet(
                    window->DrawList,
                    ImVec2(text_pos.x - text_offset_x * 0.5f, text_pos.y + g.FontSize * 0.5f),
                    text_col
                );
            else if (!is_leaf)
                RenderArrow(
                    window->DrawList,
                    ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y + g.FontSize * 0.15f),
                    text_col,
                    is_open ? ((flags & ImGuiTreeNodeFlags_UpsideDownArrow) ? ImGuiDir_Up
                                                                            : ImGuiDir_Down)
                            : ImGuiDir_Right,
                    0.70f
                );
            if (g.LogEnabled) LogSetNextTextDecoration(">", NULL);
        }

        if (span_all_columns) TablePopBackgroundChannel();

        // Label
        if (display_frame)
            RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
        else
            RenderText(text_pos, label, label_end, false);
    }

    // if (store_tree_node_stack_data && is_open)
    //     TreeNodeStoreStackData(flags); // Call before TreePushOverrideID()
    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        TreePushOverrideID(id); // Could use TreePush(label) but this avoid computing twice

    IMGUI_TEST_ENGINE_ITEM_INFO(
        id,
        label,
        g.LastItemData.StatusFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) |
            (is_open ? ImGuiItemStatusFlags_Opened : 0)
    );
    return is_open;
}

} // namespace ImGui