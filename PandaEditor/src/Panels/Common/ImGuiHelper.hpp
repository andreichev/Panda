//
// Created by Michael Andreichev on 20.12.2023.
//

#pragma once

#include <Panda/ImGui/Colors.hpp>
#include <Panda/Base/Base.hpp>
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
void drawVec3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.f);
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

} // namespace Panda