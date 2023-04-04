#pragma once

#include "Panda/Events/Event.hpp"

#include <string>

namespace Panda {

class Layer {
public:
    Layer(const std::string name = "Layer");
    virtual ~Layer() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate(double deltaTime) {}
    virtual void onImGuiRender() {}
    virtual void onEvent(Event *event) {}

    const std::string &getName() const {
        return m_debugName;
    }

protected:
    std::string m_debugName;
};

} // namespace Panda