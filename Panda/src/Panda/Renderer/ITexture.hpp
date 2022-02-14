//
// Created by Admin on 11.02.2022.
//

#pragma once

namespace Panda {
class ITexture {
public:
    virtual ~ITexture() = default;
    virtual void bind(unsigned int slot) = 0;
    virtual void unbind() = 0;
    // TODO: Add mipmapping functional
};
} // namespace Panda
