namespace Foundation {

#define PND_STATIC_ASSERT(_condition, ...) static_assert(_condition, "" __VA_ARGS__)

#define PND_ASSERT(_condition, _format, ...)                                                                                               \
    if (_condition == false) {                                                                                                             \
        PND_CRITICAL("CHECK " _format, ##__VA_ARGS__);                                                                                     \
        assert(false);                                                                                                                     \
    }

} // namespace Foundation