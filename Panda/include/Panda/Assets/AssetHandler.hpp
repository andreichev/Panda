#pragma once

#include "Asset.hpp"
#include "Panda/Serialization/FileStream.hpp"

#include <Miren/Base.hpp>
#include <string>
#include <filesystem>

namespace Panda {

using path_t = std::filesystem::path;

struct AssetPackFileInfo {
    uint64_t size;
    uint64_t offset;
    AssetType type;
};

class AssetHandler {
public:
    virtual ~AssetHandler() = default;

    virtual Foundation::Shared<Asset> loadEditor(const path_t &filePath) = 0;

    virtual void saveRuntime(const FileStreamReader &fileStream, AssetId id) {
        LOG_CRITICAL("NOT IMPLEMENTED");
    }
    virtual Foundation::Shared<Asset>
    loadRuntime(const FileStreamReader &fileStream, AssetPackFileInfo info) {
        LOG_CRITICAL("NOT IMPLEMENTED");
    }
};

} // namespace Panda