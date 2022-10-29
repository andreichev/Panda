//
// Created by Admin on 18.02.2022.
//

#include "Miren/PlatformData.hpp"

namespace Panda {

PlatformData *PlatformData::get() {
    static PlatformData platformData;
    return &platformData;
}

void PlatformData::setResourcesPath(std::string path) {
    resourcesPath = path + "/";
}

std::string &PlatformData::getResourcesPath() {
    return resourcesPath;
}

} // namespace Panda