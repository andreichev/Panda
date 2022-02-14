//
// Created by Admin on 22.01.2022.
//

#pragma once

#include "Panda/Base/GSize.hpp"
#include "Panda/Base/Base.hpp"
#include "Panda/GameLogic/ILevel.hpp"

namespace Panda {

struct ApplicationStartupSettings;

class ApplicationStartupSettingsBuilder {
private:
    const char *m_name;
    const char *m_windowTitle;
    GSize m_windowSize;
    bool m_isFullScreen;
    ILevel *m_startupLevel;

public:
    ApplicationStartupSettingsBuilder();

    ApplicationStartupSettingsBuilder &name(const char *name);

    ApplicationStartupSettingsBuilder &windowTitle(const char *title);

    ApplicationStartupSettingsBuilder &windowSize(GSize size);

    ApplicationStartupSettingsBuilder &isFullScreen(bool value);

    ApplicationStartupSettingsBuilder &startupLevel(ILevel *level);

    Unique<ApplicationStartupSettings> build();
};

struct ApplicationStartupSettings {
public:
    const char *name;
    const char *windowTitle;
    GSize windowSize;
    bool isFullScreen;
    ILevel *startupLevel;

    static Unique<ApplicationStartupSettingsBuilder> builder();

    ApplicationStartupSettings(const char *name, const char *windowTitle, const GSize &windowSize, bool isFullScreen, ILevel *startupLevel);
};

} // namespace Panda