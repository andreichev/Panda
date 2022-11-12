#pragma once

#include "Config.hpp"
#include "VertexBufferLayoutData.hpp"

#include <cstdlib>

#if MIREN_LOG_ENABLED == 1
#    define MIREN_LOG(x) LOG_INFO(x)
#else
#    define MIREN_LOG(x)
#endif

#if MIREN_CMDBUF_LOG_ENABLED == 1
#    define CMDBUF_LOG(x) LOG_INFO(x)
#else
#    define CMDBUF_LOG(x)
#endif

namespace Miren {

enum class UniformDataType { Int, Mat4 };

using ShaderHandle = uint16_t;
using TextureHandle = uint16_t;
using IndexBufferHandle = uint16_t;
using VertexBufferHandle = uint16_t;
using VertexLayoutHandle = uint16_t;

#define MIREN_INVALID_HANDLE UINT16_MAX

struct TransientIndexBuffer {
    TransientIndexBuffer()
        : data(nullptr)
        , size(0)
        , startIndex(0)
        , handle(MIREN_INVALID_HANDLE)
        , elementType(BufferElementType::UnsignedInt) {}
    uint8_t *data;
    uint32_t size;
    uint32_t startIndex;
    IndexBufferHandle handle;
    BufferElementType elementType;
};

struct TransientVertexBuffer {
    TransientVertexBuffer()
        : data(nullptr)
        , size(0)
        , startVertex(0)
        , stride(0)
        , handle(MIREN_INVALID_HANDLE)
        , layoutHandle(MIREN_INVALID_HANDLE) {}
    uint8_t *data;
    uint32_t size;
    uint32_t startVertex;
    uint16_t stride;
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
};

struct DynamicVertexBuffer {
    VertexBufferHandle m_handle;
    uint32_t m_offset;
    uint32_t m_size;
    uint32_t m_startVertex;
    uint32_t m_numVertices;
    uint16_t m_stride;
    VertexLayoutHandle m_layoutHandle;
};

struct DynamicIndexBuffer {
    IndexBufferHandle m_handle;
    uint32_t m_offset;
    uint32_t m_size;
    uint32_t m_startIndex;
};

struct Size {
    float width;
    float height;

    Size()
        : width(0)
        , height(0) {}

    Size(float width, float height)
        : width(width)
        , height(height) {}

    inline bool isZero() {
        return width == 0 && height == 0;
    }
};

struct Point {
    float x;
    float y;

    Point()
        : x(0)
        , y(0) {}

    Point(float x, float y)
        : x(x)
        , y(y) {}

    inline bool isZero() {
        return x == 0 && y == 0;
    }
};

struct Rect {
    Point origin;
    Size size;

    Rect()
        : origin()
        , size() {}

    Rect(float x, float y, float width, float height)
        : origin(x, y)
        , size(width, height) {}

    inline static Rect zero() {
        return Rect();
    }

    inline bool isZero() {
        return origin.isZero() && size.isZero();
    }
};

} // namespace Miren