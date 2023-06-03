//
//  Application.cpp
//  Main
//

#include "EditorLayer/EditorLayer.hpp"

#include <Panda.hpp>

int startApp(int argc, char **argv) {
    Panda::ApplicationStartupSettings settings;
    settings.name = "Panda Editor";
    settings.windowTitle = "Panda Editor";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    settings.startupLayer = new Panda::EditorLayer();
    runPandaApplication(settings);
    return 0;
}
