#pragma once

#include "RecentProject.hpp"

namespace Panda {

/// Состояние окна при последнем открытом проекте. Не должно сохраняться, если открыто стартовое
/// меню. Должно сохраняться только при открытом редакторе
struct LastOpenedProjectWindowState final : public Rain::Codable {
    bool isFullScreen;
    int width;
    int height;

    bool isSizeValid() {
        return width > 50 && height > 50;
    }

    RAIN_FIELDS_BEGIN(LastOpenedProjectWindowState)
    RAIN_FIELD(isFullScreen)
    RAIN_FIELD(width)
    RAIN_FIELD(height)
    RAIN_FIELDS_END
};

struct EditorSettings final : public Rain::Codable {
    std::vector<RecentProject> recentProjects;
    bool hasOpenedProject;
    LastOpenedProjectWindowState windowState;

    RAIN_FIELDS_BEGIN(EditorSettings)
    RAIN_FIELD(recentProjects)
    RAIN_FIELD(hasOpenedProject)
    RAIN_FIELD(windowState)
    RAIN_FIELDS_END
};

} // namespace Panda