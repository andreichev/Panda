//
// Created by Andreichev Mikhail on 24.07.2025.
//

#pragma once

#include "UI/Properties/EntityComponents/ComponentsDraw.hpp"

namespace Panda {

class PropertiesPanel final {
public:
    PropertiesPanel(ComponentsDrawOutput *componentsDrawOutput);
    void onImGuiRender();

private:
    ComponentsDraw m_componentsDraw;
};

} // namespace Panda