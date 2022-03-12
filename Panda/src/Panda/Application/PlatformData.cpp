//
// Created by Admin on 18.02.2022.
//

#include "PlatformData.hpp"

namespace Panda {

PlatformData &PlatformData::get() {
    static PlatformData platformData;
    return platformData;
}

} // namespace Panda