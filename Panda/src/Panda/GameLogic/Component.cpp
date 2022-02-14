//
// Created by Admin on 09.02.2022.
//

#include "pndpch.hpp"
#include "Component.hpp"

namespace Panda {

Component::Component()
    : isActive(true)
    , m_entity(nullptr) {}

void Component::setEntity(Entity *entity) {
    m_entity = entity;
}

Entity &Component::getEntity() {
    if (m_entity == nullptr) {
        PND_ERROR("Инициализация компонента должна происходить в методе initialize(), а не в конструкторе");
    }
    return *m_entity;
}

} // namespace Panda