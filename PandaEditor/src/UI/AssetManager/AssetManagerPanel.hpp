//
// Created by Andreichev Mikhail on 03.10.2025.
//

#pragma once

#include <Panda/Assets/Base/AssetInfo.hpp>

namespace Panda {

class ProjectLoader;

class AssetManagerPanel {
public:
    AssetManagerPanel(ProjectLoader *projectLoader);
    void onImGuiRender();

private:
    void drawAssetRow(AssetInfo asset);
    bool isAssetValid(const AssetInfo &info);
    bool isAssetSelected(const AssetInfo &info);

    ProjectLoader *m_projectLoader;
};

} // namespace Panda
