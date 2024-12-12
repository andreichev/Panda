//
// Created by Michael Andreichev on 20.12.2023.
//

#pragma once

#include <Panda/ImGui/Colors.hpp>
#include <Panda/ScriptEngine/ExternalScript.hpp>
#include <Panda/Base/Base.hpp>
#include <Panda/Renderer/Texture.hpp>
#include <imgui_internal.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <string>

namespace Panda {

const char *generateID();
void pushID();
void popID();
bool beginTreeNode(const char *name, bool defaultOpen);
void endTreeNode();
void shiftCursorX(float distance);
void shiftCursorY(float distance);
void shiftCursor(float x, float y);
void separator(ImVec2 size, ImVec4 color);
bool dragFloat(
    const std::string &label,
    float *v,
    float v_speed = 1.0f,
    float v_min = 0.0f,
    float v_max = 0.0f,
    const char *format = "%.3f",
    ImGuiSliderFlags flags = 0
);
bool dragFloat2(
    const std::string &label,
    float v[2],
    float v_speed = 1.0f,
    float v_min = 0.0f,
    float v_max = 0.0f,
    const char *format = "%.3f",
    ImGuiSliderFlags flags = 0
);
bool dragFloat3(
    const std::string &label,
    float v[3],
    float v_speed = 1.0f,
    float v_min = 0.0f,
    float v_max = 0.0f,
    const char *format = "%.3f",
    ImGuiSliderFlags flags = 0
);
bool dragFloat4(
    const std::string &label,
    float v[4],
    float v_speed = 1.0f,
    float v_min = 0.0f,
    float v_max = 0.0f,
    const char *format = "%.3f",
    ImGuiSliderFlags flags = 0
);
bool dragInt(
    const std::string &label,
    int *v,
    float v_speed = 1.0f,
    int v_min = 0,
    int v_max = 0,
    const char *format = "%d",
    ImGuiSliderFlags flags = 0
); // If v_min >= v_max we have no bound
bool dragInt2(
    const std::string &label,
    int v[2],
    float v_speed = 1.0f,
    int v_min = 0,
    int v_max = 0,
    const char *format = "%d",
    ImGuiSliderFlags flags = 0
);
bool dragInt3(
    const std::string &label,
    int v[3],
    float v_speed = 1.0f,
    int v_min = 0,
    int v_max = 0,
    const char *format = "%d",
    ImGuiSliderFlags flags = 0
);
bool dragInt4(
    const std::string &label,
    int v[4],
    float v_speed = 1.0f,
    int v_min = 0,
    int v_max = 0,
    const char *format = "%d",
    ImGuiSliderFlags flags = 0
);
bool checkbox(const std::string &label, bool *v);
int combo(const std::string &label, const std::vector<std::string> &list, int current);
bool drawVec3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.f);
void separator();
void beginDisabled(bool disabled = true);
bool isItemDisabled();
void endDisabled();
inline ImRect getItemRect();
inline ImRect rectExpanded(const ImRect &rect, float x, float y);
void underline(bool fullWidth = false, float offsetX = 0.0f, float offsetY = -1.0f);
void drawItemActivityOutline(
    float rounding = 0.0f,
    bool drawWhenInactive = false,
    ImColor colourWhenActive = ImColor(80, 80, 80)
);
void beginPropertiesGrid(int columns = 2);
void endPropertiesGrid();
bool propertyColor(const char *label, Color &value);
bool propertyTexture(const char *label, UUID &textureId, Foundation::Shared<Asset> asset);
bool drawFieldValue(ScriptField &field);

} // namespace Panda