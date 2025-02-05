#ifndef IMGUI_DEFINE_MATH_OPERATORS
#    define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <imgui.h>

namespace ImGui {

bool DragScalarCustom(
    const char *label,
    ImGuiDataType data_type,
    void *p_data,
    float v_speed,
    const void *p_min = nullptr,
    const void *p_max = nullptr,
    const char *format = nullptr,
    ImGuiSliderFlags flags = 0
);

bool DragScalarCustomN(
    const char *label,
    ImGuiDataType data_type,
    void *p_data,
    int components,
    float v_speed,
    const void *p_min,
    const void *p_max,
    const char *format,
    ImGuiSliderFlags flags
);

} // namespace ImGui