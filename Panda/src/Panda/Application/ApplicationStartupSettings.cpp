//
// Created by Admin on 01.02.2022.
//

#include "ApplicationStartupSettings.hpp"

namespace Panda {

ApplicationStartupSettingsBuilder::ApplicationStartupSettingsBuilder()
    : m_name("Panda Sandbox App")
    , m_windowTitle("Panda")
    , m_windowSize(Size(600, 400))
    , m_isFullScreen(false)
    , m_startupLevel(nullptr) {}

ApplicationStartupSettingsBuilder &ApplicationStartupSettingsBuilder::name(const char *name) {
    m_name = name;
    return *this;
}

ApplicationStartupSettingsBuilder &ApplicationStartupSettingsBuilder::windowTitle(const char *title) {
    m_windowTitle = title;
    return *this;
}
ApplicationStartupSettingsBuilder &ApplicationStartupSettingsBuilder::windowSize(Size size) {
    m_windowSize = size;
    return *this;
}
ApplicationStartupSettingsBuilder &ApplicationStartupSettingsBuilder::isFullScreen(bool value) {
    m_isFullScreen = value;
    return *this;
}

ApplicationStartupSettings ApplicationStartupSettingsBuilder::build() {
    return ApplicationStartupSettings(m_name, m_windowTitle, m_windowSize, m_isFullScreen, m_startupLevel);
}

ApplicationStartupSettingsBuilder &ApplicationStartupSettingsBuilder::startupLevel(Level *level) {
    m_startupLevel = level;
    return *this;
}

ApplicationStartupSettingsBuilder ApplicationStartupSettings::builder() {
    return ApplicationStartupSettingsBuilder();
}

ApplicationStartupSettings::ApplicationStartupSettings(
    const char *name, const char *windowTitle, const Size &windowSize, bool isFullScreen, Level *startupLevel)
    : name(name)
    , windowTitle(windowTitle)
    , windowSize(windowSize)
    , isFullScreen(isFullScreen)
    , startupLevel(startupLevel) {}

} // namespace Panda
