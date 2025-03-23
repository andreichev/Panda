//
//  Application.cpp
//  Main
//

#include "EditorLayer/EditorLayer.hpp"

#include <Panda.hpp>

int startApp(int argc, char **argv) {
    auto application = new Panda::Application();
    application->pushLayer(new Panda::EditorLayer);
    application->loop();
    delete application;
    return 0;
}
