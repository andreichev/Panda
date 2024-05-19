#include "Rain/GlobalData.hpp"

namespace Rain {

GlobalData &getGlobalData() {
    static GlobalData globalData;
    return globalData;
}

} // namespace Rain