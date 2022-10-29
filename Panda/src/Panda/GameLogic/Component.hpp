//
// Created by Admin on 09.02.2022.
//

#pragma once

#include <Foundation/Foundation.hpp>

namespace Panda {

class Entity;

class Component {
public:
    Component();
    virtual ~Component() = default;
    void setEntity(Entity *entity);
    Entity &getEntity();
    virtual void initialize() = 0;
    virtual void update(double deltaTime) = 0;
    bool isActive;

private:
    Entity *m_entity;
};

} // namespace Panda