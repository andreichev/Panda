#pragma once

#include <Foundation/Foundation.hpp>
#include <Foundation/Logger.hpp>
#include "Panda/ImGui/FontAwesome.h"
#include <imgui.h>
#include <cctype>
#include <cstdlib>
#include <array>
#include <sstream>

namespace Panda {

struct ConsolePanel {
    using MessageType = Foundation::EditorLogger::MessageType;
    ConsolePanel() {
        clearLog();
        index = 0;
        countInfo = 0;
        countWarning = 0;
        countError = 0;
        autoScroll = true;
        scrollToBottom = false;
        loop = false;
        PND_ASSERT(s_instance == nullptr, "ONLY SINGE CONSOLE SUPPORTED");
        s_instance = this;
    }

    ~ConsolePanel() {
        clearLog();
        s_instance = nullptr;
    }

    static void loggerCallback(std::string_view message, MessageType type) {
        if (!s_instance) { return; }
        s_instance->addLog(message, type);
    }

    void onImGuiRender() {
        if (!ImGui::Begin("Console")) {
            ImGui::End();
            return;
        }
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);
        ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_O, ImGuiInputFlags_Tooltip);
        if (ImGui::Button("Options")) { ImGui::OpenPopup("Options"); }
        ImGui::SameLine();
        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Auto-scroll", &autoScroll);
            ImGui::EndPopup();
        }
        if (ImGui::Button("Clear")) { clearLog(); }
        ImGui::SameLine();

        std::stringstream s;
        s << getString(ICON_INFO_CIRCLE).c_str() << " " << countInfo;
        if (filterMode[0]) {
            ImGui::SetItemAllowOverlap();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            if (ImGui::Button(s.str().c_str())) { filterMode[0] = false; }
            ImGui::PopStyleColor();
        } else if (ImGui::Button(s.str().c_str())) {
            filterMode[0] = !filterMode[0];
        }
        ImGui::SameLine();
        s.str("");
        s << getString(ICON_EXCLAMATION_TRIANGLE).c_str() << " " << countWarning;
        if (filterMode[1]) {
            ImGui::SetItemAllowOverlap();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            if (ImGui::Button(s.str().c_str())) { filterMode[1] = false; }
            ImGui::PopStyleColor();
        } else if (ImGui::Button(s.str().c_str())) {
            filterMode[1] = !filterMode[1];
        }
        ImGui::SameLine();
        s.str("");
        s << getString(ICON_TIMES_CIRCLE).c_str() << " " << countError;
        if (filterMode[2]) {
            ImGui::SetItemAllowOverlap();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            if (ImGui::Button(s.str().c_str())) { filterMode[2] = false; }
            ImGui::PopStyleColor();
        } else if (ImGui::Button(s.str().c_str())) {
            filterMode[2] = !filterMode[2];
        }
        ImGui::Separator();

        if (ImGui::BeginChild(
                "ScrollingRegion",
                ImVec2(0, 0),
                ImGuiChildFlags_None,
                ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NavFlattened
            )) {
            if (ImGui::BeginPopupContextWindow()) {
                if (ImGui::Selectable("Clear")) clearLog();
                ImGui::EndPopup();
            }
            int i;
            if (index < count) {
                i = MAX_LINES - (count - index);
            } else {
                i = index - count;
            }
            for (int c = 0; c < count; c++) {
                ImVec4 textColor;
                auto type = data[i].type;
                switch (type) {
                    case MessageType::INFO:
                        textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                        break;
                    case MessageType::WARNING:
                        textColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                        break;
                    case MessageType::ERROR:
                        textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                        break;
                }
                bool canShow = !filterMode[0] && !filterMode[1] && !filterMode[2];
                if (filterMode[0] && type == MessageType::INFO) { canShow = true; }
                if (filterMode[1] && type == MessageType::WARNING) { canShow = true; }
                if (filterMode[2] && type == MessageType::ERROR) { canShow = true; }
                if (!canShow) {
                    i++;
                    i %= MAX_LINES;
                    continue;
                }

                ImGui::PushStyleColor(ImGuiCol_Text, textColor);
                ImGui::Text("[%s] %s", getTimeToString(data[i].time).data(), data[i].log);
                ImGui::PopStyleColor();
                i++;
                i %= MAX_LINES;
            }

            if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.0f);
            }
        }
        ImGui::PopStyleVar(2);
        ImGui::EndChild();
        ImGui::End();
    }

private:
    static constexpr int32_t MAX_LINES = 100;
    static constexpr int32_t MAX_LINE_SYMBOLS = 100;
    static ConsolePanel *s_instance;

    static std::string getTimeToString(time_t &time) {
        std::tm *tm = std::localtime(&time);
        auto h = tm->tm_hour, m = tm->tm_min, s = tm->tm_sec;
        std::stringstream res;
        if (h < 10) {
            res << '0' << h;
        } else {
            res << h;
        }
        res << ':';
        if (m < 10) {
            res << '0' << m;
        } else {
            res << m;
        }
        res << ':';
        if (s < 10) {
            res << '0' << s;
        } else {
            res << s;
        }
        return res.str();
    }
    struct Log {
        char log[MAX_LINE_SYMBOLS];
        MessageType type;
        time_t time;
    };

    void clearLog() {
        loop = false;
        countInfo = 0;
        countWarning = 0;
        countError = 0;
        index = 0;
        count = 0;
    }

    void addLog(std::string_view message, MessageType type) {
        if (message.size() > MAX_LINE_SYMBOLS) {
            LOG_WARN("Message too long!");
            return;
        }
        // TODO: fix bugs
        if (loop) {
            countInfo -= data[index].type == MessageType::INFO;
            countWarning -= data[index].type == MessageType::WARNING;
            countError -= data[index].type == MessageType::ERROR;
            countInfo = Foundation::max(0, countInfo);
            countWarning = Foundation::max(0, countWarning);
            countError = Foundation::max(0, countError);
        }
        countInfo += type == MessageType::INFO;
        countWarning += type == MessageType::WARNING;
        countError += type == MessageType::ERROR;

        memccpy(data[index].log, message.data(), '\0', message.length());
        data[index].log[message.length()] = '\0';
        data[index].type = type;
        data[index].time = std::time(nullptr);
        index++;
        if (index == MAX_LINES) { loop = true; }
        index %= MAX_LINES;
        count++;
        count = Foundation::min(count, MAX_LINES);
    }

    std::array<Log, MAX_LINES> data;

    bool loop;
    bool autoScroll;
    bool filterMode[3] = {false, false, false};
    int32_t countInfo;
    int32_t countWarning;
    int32_t countError;
    bool scrollToBottom;
    int32_t index;
    int32_t count;
};

} // namespace Panda