//
// Created by Admin on 14.03.2022.
//

#pragma once

#include "Miren/Base.hpp"
#include "Miren/VertexBufferLayoutData.hpp"
#include "RenderDraw.hpp"

namespace Miren {

/// Класс используемый для хранения данных о кадре.
/// Тут хранятся буферы, которые существуют только один кадр и данные о графических вызовах
class Frame {
public:
    Frame();
    void beginDrawCall();
    void setState(uint32_t state);
    void setIndexBuffer(IndexBufferHandle handle, intptr_t offset, uint32_t count);
    void setVertexBuffer(VertexBufferHandle handle, intptr_t offset);
    void setShader(ShaderHandle handle);
    void setVertexLayout(VertexLayoutHandle handle);
    void setIsIndexed(bool value);
    void setNumberOfElements(uint32_t count);
    void setScissorRect(Rect rect);
    void setUniform(ShaderHandle handle, const char *name, void *value, UniformDataType type);
    void setTexture(TextureHandle textureHandle, uint32_t slot);
    void submitCurrentDrawCall();
    uint32_t getDrawCallsCount();
    RenderDraw *getDrawCalls();
    void reset();

    TransientIndexBuffer m_transientIb;
    TransientVertexBuffer m_transientVb;
    uint32_t m_transientVbSize;
    uint32_t m_transientIbSize;
    Rect m_viewport;

private:
    int m_drawCallsCount;
    RenderDraw m_drawCalls[MAX_DRAW_CALLS];
};

} // namespace Miren
