//
// Created by Admin on 14.03.2022.
//

#pragma once

#include "Miren/Base.hpp"
#include "Miren/VertexBufferLayoutData.hpp"
#include "RenderDraw.hpp"

#include <queue>

namespace Panda {

/// Класс используемый для хранения данных о кадре.
/// Тут хранятся буферы, которые существуют только один кадр и данные о графических вызовах
class Frame {
public:
    Frame();
    void beginDrawCall();
    RenderDraw *popDrawCall();
    void setState(uint32_t state);
    void setIndexBuffer(IndexBufferHandle handle, void *offset, uint32_t count);
    void setVertexBuffer(VertexBufferHandle handle);
    void setShader(ShaderHandle handle);
    void setIsIndexed(bool value);
    void setNumberOfElements(uint32_t count);
    void setScissorRect(Rect rect);
    void free(const RenderDraw *draw);
    void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type);
    void setTexture(TextureHandle textureHandle, uint32_t slot);
    void submitCurrentDrawCall();
    TransientVertexBuffer *createTransientVertexBuffer(uint32_t size, VertexBufferLayoutData *layout);
    uint32_t getDrawCallsCount();

    TransientIndexBuffer *m_transientIb;
    TransientVertexBuffer *m_transientVb;
    uint32_t m_transientVbOffset;

private:
    std::queue<RenderDraw *> m_drawCalls;
};

} // namespace Panda
