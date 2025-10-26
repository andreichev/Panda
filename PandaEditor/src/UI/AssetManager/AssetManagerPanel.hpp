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
    std::string getAssetName(const AssetInfo &info);
    bool isAssetSelected(const AssetInfo &info);
    void selectAsset(AssetId id);
    void unselectAsset(AssetId id);
    void deleteSelectedAssets();

    ProjectLoader *m_projectLoader;
    int m_rowIndex;
    int m_firstSelectedRow;
    int m_lastSelectedRow;
    bool m_shiftSelectionRunning;
};

} // namespace Panda
