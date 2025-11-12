#pragma once

#include <functional>

class Defer {
public:
    Defer(std::function<void()> func)
        : func(func) {}
    ~Defer() {
        if (func) func();
    }

private:
    std::function<void()> func;
};

#define DEFER(code) Defer defer([&]() { code; })