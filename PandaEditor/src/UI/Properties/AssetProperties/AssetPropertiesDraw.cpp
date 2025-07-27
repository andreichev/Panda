//
// Created by Andreichev Mikhail on 27.07.2025.
//

#include "AssetPropertiesDraw.hpp"
#include "ProjectLoader/AssetHandlerEditor.hpp"

#include <Panda/GameLogic/GameContext.hpp>
#include <Panda/Assets/Base/Asset.hpp>
#include <Panda/ImGui/ImGuiFonts.hpp>
#include <imgui.h>

namespace Panda {

void AssetPropertiesDraw::drawProperties(const path_t &assetPath) {
    AssetHandlerEditor *assetHandler =
        static_cast<AssetHandlerEditor *>(GameContext::getAssetHandler());
    AssetId assetId = assetHandler->getAssetId(assetPath);
    if (!assetId) {
        if (assetHandler->canImport(assetPath)) {
            ImGui::Text("Asset %s is not imported", assetPath.filename().string().c_str());
            if (ImGui::Button("Import")) { assetHandler->registerAsset(assetPath); }
        } else {
            ImGui::Text("Extension %s is not supported", assetPath.extension().string().c_str());
        }
        return;
    }
    AssetInfo info = assetHandler->getInfo(assetId);
    switch (info.type) {
        case AssetType::TEXTURE: {
            Fonts::pushFont("Bold");
            ImGui::Text("Texture %s", assetPath.filename().string().c_str());
            Fonts::popFont();
            break;
        }
        case AssetType::SHADER: {
            Fonts::pushFont("Bold");
            ImGui::Text("Shader %s", assetPath.filename().string().c_str());
            Fonts::popFont();
            break;
        }
        case AssetType::MATERIAL: {
            Fonts::pushFont("Bold");
            ImGui::Text("Material %s", assetPath.filename().string().c_str());
            Fonts::popFont();
            break;
        }
        case AssetType::CUBE_MAP:
        case AssetType::NONE:
            break;
    }
}

} // namespace Panda
