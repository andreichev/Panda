//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Renderer/ITexture.hpp"

namespace Panda {

class OpenGLTexture : public ITexture {
public:
    ~OpenGLTexture() override;
    explicit OpenGLTexture(const char *path);

private:
    void bind(unsigned int slot) override;
    void unbind() override;

private:
    unsigned int id;
};

} // namespace Panda
