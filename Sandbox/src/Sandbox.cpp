//
//  Application.cpp
//  Sandbox
//
//  Created by Admin on 22.07.2021.
//

#include "sndbxpch.hpp"
#include <Panda/Application/Application.hpp>
#include <Panda/Application/ApplicationStartupSettings.hpp>
#include "Levels/BaseLevel.hpp"

int main() {
    Panda::Application::get().run(Panda::ApplicationStartupSettings::builder()
                                      ->name("ITIS Craft")
                                      .windowTitle("ITIS Craft")
                                      .windowSize({900, 600})
                                      .isFullScreen(true)
                                      .startupLevel(new BaseLevel())
                                      .build());
}
