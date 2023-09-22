#pragma once

#include <glm/glm.hpp>
#include <Miren/Miren.hpp>

namespace Panda {

struct MeshData {
    Miren::VertexLayoutHandle layoutHandle;
    Foundation::Memory vertexBuffer;
    uint32_t vertexBufferSize;
    Foundation::Memory indices;
    uint32_t indicesCount;

    MeshData(Miren::VertexLayoutHandle layoutHandle,
        Foundation::Memory vertexBuffer,
        uint32_t vertexBufferSize,
        Foundation::Memory indices,
        uint32_t indicesCount)
        : layoutHandle(layoutHandle)
        , vertexBuffer(vertexBuffer)
        , vertexBufferSize(vertexBufferSize)
        , indices(indices)
        , indicesCount(indicesCount) {}
};

} // namespace Panda