//
// Created by Andreichev Mikhail on 24.07.2025.
//

#pragma once

#include "UI/Properties/AssetProperties/AssetPropertiesDraw.hpp"
#include "UI/Properties/EntityComponents/ComponentsDraw.hpp"

namespace Panda {

class PropertiesPanel final {
public:
    PropertiesPanel(
        ComponentsDrawOutput *componentsDrawOutput, AssetPropertiesDrawOutput *assetPropertiesOutput
    );
    void onImGuiRender();

private:
    ComponentsDraw m_componentsDraw;
    AssetPropertiesDraw m_assetPropertiesDraw;
};

} // namespace Panda