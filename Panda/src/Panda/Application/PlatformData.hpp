//
// Created by Admin on 18.02.2022.
//

#pragma once

namespace Panda {

// Указатели на главные элементы платформы (окно, контекст для рендера и т. д.)
class PlatformData {
public:
    static PlatformData *get();
    void setResourcesPath(std::string path);
    std::string &getResourcesPath();
    void *layer;
    void *nativeWindowHandle;
    void *renderingContext;

private:
    std::string resourcesPath;
};

} // namespace Panda
