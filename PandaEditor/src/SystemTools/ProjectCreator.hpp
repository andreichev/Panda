//
// Created by Michael Andreichev on 08.09.2024.
//

#pragma once

#include "SystemTools.hpp"

namespace Panda {

class ProjectCreator {
public:
    void createProject(const std::string &name, const path_t &path);
};

} // namespace Panda
