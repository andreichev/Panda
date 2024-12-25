#pragma once

#include <Foundation/Foundation.hpp>
#include <Foundation/Logger.hpp>
#include <imgui.h>
#include <cctype>
#include <cstdlib>

namespace Panda {

struct ConsolePanel {
    using MessageType = Foundation::EditorLogger::MessageType;
    ConsolePanel() {
        clearLog();
        index = 0;
        autoScroll = true;
        scrollToBottom = false;
        PND_ASSERT(s_instance == nullptr, "ONLY SINGE CONSOLE SUPPORTED");
        s_instance = this;
    }

    ~ConsolePanel() {
        clearLog();
        s_instance = nullptr;
    }

    static void loggerCallback(std::string_view message, MessageType type) {
        if (!s_instance) {
            return;
        }
        s_instance->addLog(message, type);
    }


    void onImGuiRender() {
        if (!ImGui::Begin("Console")) {
            ImGui::End();
            return;
        }
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

        if (ImGui::Button("Clear")) {
            clearLog();
        }

        for (int i = 0; i < index; ++i) {
            ImVec4 textColor;
            switch (data[i].type) {
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
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);

            ImGui::Text("[%s] %s", getTimeToString(data[i].time).data(), data[i].log.data());
            ImGui::PopStyleColor();
        }

        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::PopStyleVar();
        ImGui::End();
    }
private:

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
        std::string log;
        MessageType type;
        time_t time;

        Log() {
            log.reserve(MAX_LINE_SYMBOLS);
        }
    };

    void clearLog() {
        index = 0;
    }

    void addLog(std::string_view message, MessageType type) {
        if (index == MAX_LINES) {
            return;
        }
        data[index].log = message;
        data[index].type = type;
        data[index].time = std::time(nullptr);
        index++;
    }

    static constexpr int32_t MAX_LINES = 50;
    static constexpr int32_t MAX_LINE_SYMBOLS = 100;
    static ConsolePanel *s_instance;

    std::array<Log, MAX_LINES> data;
    bool autoScroll;
    bool scrollToBottom;
    int32_t index;
};

} // namespace Panda