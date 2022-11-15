#pragma once

#include "Logger.hpp"

namespace Foundation {

#define STATIC_ASSERT(_condition, ...) static_assert(_condition, "" __VA_ARGS__)

#define ASSERT(_condition, _format, ...)                                                                                                   \
    if (_condition == false) {                                                                                                             \
        LOG_CRITICAL("CHECK " _format, ##__VA_ARGS__);                                                                                     \
        assert(false);                                                                                                                     \
    }

} // namespace Foundation