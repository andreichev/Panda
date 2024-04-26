//
// Created by Michael Andreichev on 11.11.2023.
//

#pragma once

#include "PandaUI/Context.hpp"

namespace PandaUI {

void initialize();

template<typename T, typename... Args>
Foundation::Shared<T> makeView(Args &&...args) {
    return Context::shared().makeView<T>(std::forward<Args>(args)...);
}

} // namespace PandaUI