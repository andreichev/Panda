//
// Created by Admin on 18.02.2022.
//

#include "Miren/PlatformData.hpp"

namespace Miren {

PlatformData *PlatformData::get() {
    static PlatformData platformData;
    return &platformData;
}

} // namespace Miren