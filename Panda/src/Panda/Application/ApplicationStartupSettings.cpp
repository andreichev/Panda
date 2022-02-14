//
// Created by Admin on 01.02.2022.
//

#include "pndpch.hpp"
#include "ApplicationStartupSettings.hpp"

namespace Panda {

ApplicationStartupSettingsBuilder::ApplicationStartupSettingsBuilder()
    : m_name("Panda Sandbox App")
    , m_windowTitle("Panda")
    , m_windowSize(GSize(600, 400))
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
ApplicationStartupSettingsBuilder &ApplicationStartupSettingsBuilder::windowSize(GSize size) {
    m_windowSize = size;
    return *this;
}
ApplicationStartupSettingsBuilder &ApplicationStartupSettingsBuilder::isFullScreen(bool value) {
    m_isFullScreen = value;
    return *this;
}

std::unique_ptr<ApplicationStartupSettings> ApplicationStartupSettingsBuilder::build() {
    return std::make_unique<ApplicationStartupSettings>(m_name, m_windowTitle, m_windowSize, m_isFullScreen, m_startupLevel);
}

ApplicationStartupSettingsBuilder &ApplicationStartupSettingsBuilder::startupLevel(ILevel *level) {
    m_startupLevel = level;
    return *this;
}

std::unique_ptr<ApplicationStartupSettingsBuilder> ApplicationStartupSettings::builder() {
    return std::make_unique<ApplicationStartupSettingsBuilder>();
}

ApplicationStartupSettings::ApplicationStartupSettings(
    const char *name, const char *windowTitle, const GSize &windowSize, bool isFullScreen, ILevel *startupLevel)
    : name(name)
    , windowTitle(windowTitle)
    , windowSize(windowSize)
    , isFullScreen(isFullScreen)
    , startupLevel(startupLevel) {}

} // namespace Panda