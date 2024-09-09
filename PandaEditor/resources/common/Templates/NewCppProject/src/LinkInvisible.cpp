extern "C" void hookFunc();

[[maybe_unused]] void linkInvisible() {
    hookFunc();
}