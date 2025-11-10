//
// Created by Andreichev Mikhail on 27.07.2025.
//

#include "UI/Common/ImGuiHelper.hpp"
#include "AssetPropertiesDraw.hpp"
#include "ProjectLoader/AssetHandlerEditor.hpp"

#include <Panda/GameLogic/GameContext.hpp>
#include <Panda/Assets/Base/Asset.hpp>
#include <Panda/ImGui/ImGuiFonts.hpp>
#include <imgui.h>

namespace Panda {

AssetPropertiesDraw::AssetPropertiesDraw(AssetPropertiesDrawOutput *output)
    : m_output(output) {}

void AssetPropertiesDraw::drawProperties(AssetId assetId) {
    AssetHandler *handler = GameContext::getAssetHandler();
    PND_ASSERT(handler != nullptr, "INVALID ASSET HANDLER");
    AssetHandlerEditor *assetHandler = static_cast<AssetHandlerEditor *>(handler);
    AssetInfo info = assetHandler->getInfo(assetId);
    std::string name = assetHandler->getAssetName(info);
    if (!assetHandler->assetFilesExist(assetId)) {
        ImGui::Text("Asset is missing");
        if (ImGui::Button("Locate")) { m_output->locateMissingAsset(assetId); }
        return;
    }
    switch (info.type) {
        case AssetType::TEXTURE: {
            TextureAssetMeta meta = std::get<TextureAssetMeta>(info.meta);
            Fonts::pushFont("Bold");
            ImGui::Text("Texture %s", name.c_str());
            Fonts::popFont();
            bool changed = false;
            const std::vector<std::string> filteringList = {
                "NEAREST",
                "LINEAR",
                "NEAREST_MIPMAP_NEAREST",
                "NEAREST_MIPMAP_LINEAR",
                "LINEAR_MIPMAP_NEAREST",
                "LINEAR_MIPMAP_LINEAR"
            };
            int minFiltering = combo("Min Filtering", filteringList, meta.minFiltering, false);
            if (minFiltering != meta.minFiltering) {
                meta.minFiltering = (Miren::TextureFiltering)minFiltering;
                changed = true;
            }
            int magFiltering = combo("Mag Filtering", filteringList, meta.magFiltering, false);
            if (magFiltering != meta.magFiltering) {
                meta.magFiltering = (Miren::TextureFiltering)magFiltering;
                changed = true;
            }
            int32_t numMips = meta.numMips;
            if (dragInt("Num Mips", &numMips, 1, 1, 3, "%d", 0, false)) {
                meta.numMips = numMips;
                changed = true;
            }
            if (changed) {
                info.meta = meta;
                assetHandler->updateInfo(assetId, info);
            }
            break;
        }
        case AssetType::SHADER: {
            ShaderAssetMeta meta = std::get<ShaderAssetMeta>(info.meta);
            Fonts::pushFont("Bold");
            ImGui::Text("Shader %s", name.c_str());
            Fonts::popFont();
            ImGui::Text("Vertex: %s", meta.vertexCodePath.string().c_str());
            ImGui::Text("Fragment: %s", meta.fragmentCodePath.string().c_str());
            break;
        }
        case AssetType::MATERIAL: {
            MaterialAssetMeta meta = std::get<MaterialAssetMeta>(info.meta);
            Fonts::pushFont("Bold");
            ImGui::Text("Material %s", name.c_str());
            Fonts::popFont();
            path_t shaderPath;
            if (meta.shader) {
                AssetInfo shaderInfo = assetHandler->getInfo(meta.shader);
                ShaderAssetMeta shaderMeta = std::get<ShaderAssetMeta>(shaderInfo.meta);
                shaderPath = assetHandler->getProjectPath() / shaderMeta.fragmentCodePath;
            }
            AssetRef<Asset> asset = AssetRef<Asset>(meta.shader);
            if (assetHandler->isLoaded(assetId)) {
                AssetRef<MaterialAsset> material = assetHandler->makeRef<MaterialAsset>(assetId);
                if (propertyShader("Shader", shaderPath, asset, false)) {
                    meta.shader = asset.getId();
                    info.meta = meta;
                    assetHandler->updateInfo(assetId, info);
                }
                bool fieldsChanged = false;
                MaterialData &materialData = material->getInputs();
                for (auto &input : materialData.inputs) {
                    switch (input.type) {
                        case MaterialFieldType::FLOAT: {
                            float value = std::get<float>(input.value);
                            fieldsChanged |= dragFloat(input.name.c_str(), &value, 0.005f);
                            input.value = value;
                            break;
                        }
                        case MaterialFieldType::VEC4:
                        case MaterialFieldType::TEXTURE:
                        case MaterialFieldType::UNKNOWN: {
                            ImGui::TextUnformatted(input.name.c_str());
                            break;
                        }
                    }
                }
                if (fieldsChanged) { material->setChanged(true); }
                if (material->isChanged()) {
                    if (ImGui::Button("Save")) {
                        if (assetHandler->saveMaterial(info, materialData)) {
                            material->setChanged(false);
                        }
                    }
                }
            } else {
                ImGui::Text(
                    "Asset is not loaded. Editing fields is available only for loaded assets."
                );
            }
            break;
        }
        case AssetType::CUBE_MAP:
        case AssetType::MESH:
        case AssetType::NONE:
            break;
    }
    if (ImGui::Button("Reimport")) { assetHandler->reload(assetId); }
}

void AssetPropertiesDraw::drawProperties(const path_t &assetPath) {
    AssetHandler *handler = GameContext::getAssetHandler();
    PND_ASSERT(handler != nullptr, "INVALID ASSET HANDLER");
    AssetHandlerEditor *assetHandler = static_cast<AssetHandlerEditor *>(handler);
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
    drawProperties(assetId);
}

} // namespace Panda
