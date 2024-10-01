#pragma once

#include <Foundation/Foundation.hpp>
#include <imgui.h>
#include <cctype>
#include <cstdlib>

namespace Panda {

struct ConsolePanel {
    static constexpr int32_t MAX_LINES = 50;
    static constexpr int32_t MAX_LINE_SYMBOLS = 1000;
    static ConsolePanel *s_instance;

    char *data;
    ImGuiTextFilter Filter;
    bool AutoScroll;
    bool ScrollToBottom;
    int32_t index;
    int32_t count;

    ConsolePanel() {
        ClearLog();
        data = (char *)F_ALLOC(Foundation::getAllocator(), MAX_LINES * MAX_LINE_SYMBOLS);
        index = 0;
        AutoScroll = true;
        ScrollToBottom = false;
        PND_ASSERT(s_instance == nullptr, "ONLY SINGE CONSOLE SUPPORTED");
        s_instance = this;
    }

    ~ConsolePanel() {
        ClearLog();
        F_FREE(Foundation::getAllocator(), data);
        s_instance = nullptr;
    }

    static void loggerCallback(std::string_view message) {
        if (!s_instance) {
            return;
        }
        s_instance->AddLog(message);
    }

    void ClearLog() {
        count = 0;
        index = 0;
    }

    void AddLog(std::string_view message) {
        size_t len = message.size();
        char *buf = data + index * MAX_LINE_SYMBOLS;
        char *str = (char *)memcpy(buf, (const void *)message.data(), len);
        str[len] = 0;
        index++;
        index %= MAX_LINES;
        count++;
        count = Foundation::min(count, MAX_LINES);
    }

    void onImGuiRender() {
        if (!ImGui::Begin("Console")) {
            ImGui::End();
            return;
        }

        if (ImGui::SmallButton("Clear")) {
            ClearLog();
        }
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy");

        ImGui::Separator();

        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Options, Filter
        ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_O, ImGuiInputFlags_Tooltip);
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::Separator();

        if (ImGui::BeginChild(
                "ScrollingRegion",
                ImVec2(0, 0),
                ImGuiChildFlags_None,
                ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NavFlattened
            )) {
            if (ImGui::BeginPopupContextWindow()) {
                if (ImGui::Selectable("Clear"))
                    ClearLog();
                ImGui::EndPopup();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
            if (copy_to_clipboard)
                ImGui::LogToClipboard();
            int i;
            if (index < count) {
                i = MAX_LINES - (count - index);
            } else {
                i = index - count;
            }
            for (int c = 0; c < count; c++) {
                char *item = data + i * MAX_LINE_SYMBOLS;
                if (!Filter.PassFilter(item)) {
                    i++;
                    i %= MAX_LINES;
                    continue;
                }
                ImGui::TextUnformatted(item);
                i++;
                i %= MAX_LINES;
            }
            if (copy_to_clipboard)
                ImGui::LogFinish();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the
            // beginning of the frame. Using a scrollbar or mouse-wheel will take away from the
            // bottom edge.
            if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        ImGui::End();
    }
};

} // namespace Panda