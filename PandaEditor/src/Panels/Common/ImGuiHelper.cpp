#include "ImGuiHelper.hpp"
#include "Model/DragDropItem.hpp"
#include "ProjectLoader/AssetHandlerEditor.hpp"

#include <Panda/ImGui/FontAwesome.h>
#include <string>

namespace Panda {

static int s_uiContextID = 0;
static uint32_t s_counter = 0;
static char s_idBuffer[16] = "##";
static float firstColumnWidth = 120.f;

const char *generateID() {
    snprintf(s_idBuffer + 2, sizeof(s_idBuffer) - 2, "%x", s_counter++);
    return s_idBuffer;
}

void pushID() {
    ImGui::PushID(s_uiContextID++);
    s_counter = 0;
}

void popID() {
    ImGui::PopID();
    s_uiContextID--;
}

bool beginTreeNode(const char *name, bool defaultOpen) {
    ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed |
                                       ImGuiTreeNodeFlags_SpanAvailWidth |
                                       ImGuiTreeNodeFlags_FramePadding;
    if (defaultOpen) {
        treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
    }
    return ImGui::TreeNodeEx(name, treeNodeFlags);
}

void endTreeNode() {
    ImGui::TreePop();
}

void shiftCursorX(float distance) {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
}

void shiftCursorY(float distance) {
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
}

void shiftCursor(float x, float y) {
    const ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
}

void separator(ImVec2 size, ImVec4 color) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, color);
    ImGui::BeginChild("sep", size);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

bool drawVec3Control(const std::string &label, glm::vec3 &values, float resetValue) {
    ImGuiIO &io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];
    bool edited = false;

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    shiftCursorY(6.0f);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    const float spacingX = 0.0f;
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    const ImVec2 buttonSize(lineHeight + 3.0f, lineHeight);
    const float inputItemWidth =
        (ImGui::GetContentRegionAvail().x - spacingX) / 3.0f - buttonSize.x;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputItemWidth);
    edited = ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f") || edited;
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputItemWidth);
    edited = ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f") || edited;
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        values.z = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputItemWidth);
    edited = ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f") || edited;

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
    return edited;
}

void separator() {
    ImGui::Separator();
}

void beginDisabled(bool disabled) {
    if (disabled) {
        ImGui::BeginDisabled(true);
    }
}

bool isItemDisabled() {
    return ImGui::GetItemFlags() & ImGuiItemFlags_Disabled;
}

void endDisabled() {
    if (GImGui->DisabledStackSize > 0) {
        ImGui::EndDisabled();
    }
}

inline ImRect getItemRect() {
    return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

inline ImRect rectExpanded(const ImRect &rect, float x, float y) {
    ImRect result = rect;
    result.Min.x -= x;
    result.Min.y -= y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
}

void drawItemActivityOutline(float rounding, bool drawWhenInactive, ImColor colourWhenActive) {
    auto *drawList = ImGui::GetWindowDrawList();
    const ImRect rect = rectExpanded(getItemRect(), 1.0f, 1.0f);
    if (ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
        drawList->AddRect(rect.Min, rect.Max, ImColor(60, 60, 60), rounding, 0, 1.5f);
    }
    if (ImGui::IsItemActive()) {
        drawList->AddRect(rect.Min, rect.Max, colourWhenActive, rounding, 0, 1.0f);
    } else if (!ImGui::IsItemHovered() && drawWhenInactive) {
        drawList->AddRect(rect.Min, rect.Max, ImColor(50, 50, 50), rounding, 0, 1.0f);
    }
}

void underline(bool fullWidth, float offsetX, float offsetY) {
    if (fullWidth) {
        if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
            ImGui::PushColumnsBackground();
        else if (ImGui::GetCurrentTable() != nullptr)
            ImGui::TablePushBackgroundChannel();
    }
    const float width = fullWidth ? ImGui::GetWindowWidth() : ImGui::GetContentRegionAvail().x;
    const ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddLine(
        ImVec2(cursor.x + offsetX, cursor.y + offsetY),
        ImVec2(cursor.x + width, cursor.y + offsetY),
        Theme::backgroundDark,
        1.0f
    );
    if (fullWidth) {
        if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
            ImGui::PopColumnsBackground();
        else if (ImGui::GetCurrentTable() != nullptr)
            ImGui::TablePopBackgroundChannel();
    }
}

/// Component properties list
void beginPropertiesGrid(int columns) {
    pushID();
    ImGui::Columns(columns, nullptr, false);
}

void endPropertiesGrid() {
    ImGui::Columns(1);
    shiftCursorY(18.0f);
    popID();
}

bool propertyColor(const char *label, Color &value) {
    shiftCursorY(6.0f);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text("%s", label);
    ImGui::NextColumn();
    ImGui::PushItemWidth(-1);
    bool modified = ImGui::ColorEdit4(generateID(), &value.r);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    return modified;
}

bool propertyTexture(const char *label, UUID &textureId, Foundation::Shared<Asset> asset) {
    bool changed = false;
    shiftCursorY(6.0f);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text("%s", label);
    ImGui::NextColumn();
    if (textureId) {
        if (asset) {
            auto texture = Foundation::SharedCast<Texture>(asset);
            float height = 55;
            float aspect = texture->getSize().width / texture->getSize().height;
            aspect = Foundation::min(aspect, 4.f);
            float width = height * aspect;
            ImGui::Image((ImTextureID)(intptr_t)texture->getHandle().id, {width, height});
        } else {
            ImGui::Button("Texture Asset", {100, 55});
        }
    } else {
        ImGui::Button("No image", {100, 55});
    }
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        DragDropItem item;
        item.type = DragDropItemType::TEXTURE;
        item.assetId = textureId;
        ImGui::SetDragDropPayload(PANDA_DRAGDROP_NAME, &item, sizeof(DragDropItem));
        ImGui::Text("Texture");
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(PANDA_DRAGDROP_NAME)) {
            PND_ASSERT(payload->DataSize == sizeof(DragDropItem), "WRONG DRAGDROP ITEM SIZE");
            DragDropItem &item = *(DragDropItem *)payload->Data;
            textureId = item.assetId;
            changed = true;
        }
        ImGui::EndDragDropTarget();
    }
    if (textureId) {
        ImGui::SameLine();
        if (ImGui::Button(getString(ICON_TRASH_O).c_str())) {
            changed = true;
            textureId = 0;
        }
    }
    ImGui::NextColumn();
    return changed;
}

} // namespace Panda