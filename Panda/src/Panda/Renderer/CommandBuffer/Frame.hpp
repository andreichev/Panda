//
// Created by Admin on 14.03.2022.
//

#pragma once

#include "RenderDraw.hpp"

namespace Panda {

class Frame {
public:
    Frame();
    void beginDrawCall();
    RenderDraw *popDrawCall();
    void setIndexBuffer(IndexBufferHandle handle, uint32_t count);
    void setVertexBuffer(VertexBufferHandle handle);
    void setShader(ShaderHandle handle);
    void setIsIndexed(bool value);
    void setNumberOfElements(uint32_t count);
    void free(const RenderDraw *draw);
    void setUniform(ShaderHandle handle, const char *name, void *value, uint16_t size);
    void setTexture(TextureHandle textureHandle, uint32_t slot);
    void submitCurrentDrawCall();
    uint32_t getDrawCallsCount();

private:
    std::queue<RenderDraw *> m_drawCalls;
};

} // namespace Panda
