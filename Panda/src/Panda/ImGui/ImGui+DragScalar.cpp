#include "Panda/ImGui/ImGui+DragScalar.hpp"

#include <imgui_internal.h>

namespace ImGui {

// Note: p_data, p_min and p_max are _pointers_ to a memory address holding the data. For a Drag
// widget, p_min and p_max are optional. Read code of e.g. DragFloat(), DragInt() etc. or examples
// in 'Demo->Widgets->Data Types' to understand how to use this function directly.
bool DragScalarCustom(
    const char *label,
    ImGuiDataType data_type,
    void *p_data,
    float v_speed,
    const void *p_min,
    const void *p_max,
    const char *format,
    ImGuiSliderFlags flags
) {
    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(
        window->DC.CursorPos,
        window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f)
    );
    const ImRect total_bb(
        frame_bb.Min,
        frame_bb.Max +
            ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f)
    );

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return false;

    // Default format string when passing NULL
    if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
    bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
    if (!temp_input_is_active) {
        // Tabbing or CTRL-clicking on Drag turns it into an InputText
        const bool clicked = hovered && IsMouseClicked(0, ImGuiInputFlags_None, id);
        const bool double_clicked =
            (hovered && g.IO.MouseClickedCount[0] == 2 && TestKeyOwner(ImGuiKey_MouseLeft, id));
        const bool make_active = (clicked || double_clicked || g.NavActivateId == id);
        if (make_active && (clicked || double_clicked)) SetKeyOwner(ImGuiKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if ((clicked && g.IO.KeyCtrl) || double_clicked ||
                (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
                temp_input_is_active = true;

        // (Optional) simple click (without moving) turns Drag into an InputText
        if (g.IO.ConfigDragClickToInputText && temp_input_allowed && !temp_input_is_active)
            if (g.ActiveId == id && hovered && g.IO.MouseReleased[0] &&
                !IsMouseDragPastThreshold(0, g.IO.MouseDragThreshold * 0.5f)) {
                g.NavActivateId = id;
                g.NavActivateFlags = ImGuiActivateFlags_PreferInput;
                temp_input_is_active = true;
            }

        if (make_active && !temp_input_is_active) {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask = (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    if (temp_input_is_active) {
        // Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
        const bool is_clamp_input =
            (flags & ImGuiSliderFlags_AlwaysClamp) != 0 &&
            (p_min == NULL || p_max == NULL || DataTypeCompare(data_type, p_min, p_max) < 0);
        return TempInputScalar(
            frame_bb,
            id,
            label,
            data_type,
            p_data,
            format,
            is_clamp_input ? p_min : NULL,
            is_clamp_input ? p_max : NULL
        );
    }

    // Draw frame
    const ImU32 frame_col = GetColorU32(
        g.ActiveId == id ? ImGuiCol_FrameBgActive
        : hovered        ? ImGuiCol_FrameBgHovered
                         : ImGuiCol_FrameBg
    );
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

    // Drag behavior
    const bool value_changed =
        DragBehavior(id, data_type, p_data, v_speed, p_min, p_max, format, flags);
    if (value_changed) MarkItemEdited(id);

    const bool mixed_value = (g.CurrentItemFlags & ImGuiItemFlags_MixedValue) != 0;

    // Display value using user-provided display format so user can add prefix/suffix/decorations to
    // the value.
    char value_buf[64];
    const char *value_buf_end =
        value_buf +
        (mixed_value
             ? ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), "---")
             : DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format));

    if (g.LogEnabled) LogSetNextTextDecoration("{", "}");
    RenderTextClipped(
        frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f)
    );

    if (label_size.x > 0.0f)
        RenderText(
            ImVec2(
                frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y
            ),
            label
        );

    IMGUI_TEST_ENGINE_ITEM_INFO(
        id,
        label,
        g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0)
    );
    return value_changed;
}

} // namespace ImGui