//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Base/GSize.hpp"
#include "Panda/Base/Base.hpp"
#include "Panda/GameLogic/Level.hpp"

namespace Panda {

struct ApplicationStartupSettings;

class ApplicationStartupSettingsBuilder {
private:
    const char *m_name;
    const char *m_windowTitle;
    GSize m_windowSize;
    bool m_isFullScreen;
    Level *m_startupLevel;

public:
    ApplicationStartupSettingsBuilder();
    ApplicationStartupSettingsBuilder &name(const char *name);
    ApplicationStartupSettingsBuilder &windowTitle(const char *title);
    ApplicationStartupSettingsBuilder &windowSize(GSize size);
    ApplicationStartupSettingsBuilder &isFullScreen(bool value);
    ApplicationStartupSettingsBuilder &startupLevel(Level *level);
    ApplicationStartupSettings build();
};

struct ApplicationStartupSettings {
public:
    const char *name;
    const char *windowTitle;
    GSize windowSize;
    bool isFullScreen;
    Level *startupLevel;

    static ApplicationStartupSettingsBuilder builder();

    ApplicationStartupSettings(const char *name, const char *windowTitle, const GSize &windowSize, bool isFullScreen, Level *startupLevel);
};

} // namespace Panda