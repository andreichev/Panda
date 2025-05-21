#pragma once

#include "Asset.hpp"
#include "Path.hpp"

#include <Miren/Base.hpp>
#include <string>

namespace Panda {

/// AssetHandler отвечает за загрузку Asset.
/// Для Editor - просто из файла, для Runtime - из AssetPack.
class AssetHandler {
public:
    virtual ~AssetHandler() = default;
    /// Загрузка метаданных Asset и самих данных.
    /// Инициализация Asset и полная подготовка для дальнейшей работы. Кеширования и
    /// повторного использования на этом этапе нет.
    virtual Foundation::Shared<Asset> load(AssetId id) = 0;
};

} // namespace Panda
