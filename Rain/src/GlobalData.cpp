#include "Rain/GlobalData.hpp"

namespace Rain {

GlobalData &getGlobalData() {
    static GlobalData globalData;
    return globalData;
}

TypeInfo &findInfo(TypeId id) {
    return getGlobalData().findInfo(id);
}

} // namespace Rain