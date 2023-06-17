#pragma once

#include "Config.hpp"
#include "VertexBufferLayoutData.hpp"

#include <Foundation/Foundation.hpp>
#include <cstdlib>

#if MIREN_LOG_ENABLED == 1
#    define MIREN_LOG(...) LOG_INFO(__VA_ARGS__)
#else
#    define MIREN_LOG(...)
#endif

#if MIREN_CMDBUF_LOG_ENABLED == 1
#    define CMDBUF_LOG(...) LOG_INFO(__VA_ARGS__)
#else
#    define CMDBUF_LOG(...)
#endif

#define MIREN_INVALID_HANDLE UINT16_MAX

namespace Miren {

#define MIREN_HANDLE(name)                                                                         \
    struct name {                                                                                  \
        name(uint16_t id)                                                                          \
            : id(id) {}                                                                            \
        name()                                                                                     \
            : id(MIREN_INVALID_HANDLE) {}                                                          \
        uint16_t id;                                                                               \
    };

using ViewId = uint16_t;
MIREN_HANDLE(ShaderHandle)
MIREN_HANDLE(TextureHandle)
MIREN_HANDLE(IndexBufferHandle)
MIREN_HANDLE(FrameBufferHandle)
MIREN_HANDLE(VertexBufferHandle)
MIREN_HANDLE(VertexLayoutHandle)

enum TextureFormat {
    None = 0,
    // Color
    RGB8,
    RGBA8,
    RED_INTEGER,
    // Depth/stencil
    DEPTH24STENCIL8
};

struct TextureCreate {
    TextureFormat m_format;
    uint16_t m_width;
    uint16_t m_height;
    uint8_t m_numMips;
    Foundation::Memory m_data;

    TextureCreate()
        : m_format(TextureFormat::None)
        , m_width(1)
        , m_height(1)
        , m_numMips(1)
        , m_data(nullptr) {}
};

enum class UniformDataType { Int, Mat4, IntArray };

struct FrameBufferAttachment {
    FrameBufferAttachment()
        : handle(MIREN_INVALID_HANDLE) {}

    FrameBufferAttachment(TextureHandle handle)
        : handle(handle) {}

    TextureHandle handle;
};

struct FrameBufferSpecification {
    FrameBufferSpecification()
        : attachments()
        , num(0) {}

    FrameBufferSpecification(FrameBufferAttachment *_attachments, uint32_t num)
        : attachments()
        , num(num) {
        PND_ASSERT(num <= MAX_FRAMEBUFFER_ATTACHMENTS, "MAX ATTACHMENTS NUMBER EXCEEDED");
        for (int i = 0; i < num; i++) {
            attachments[i] = _attachments[i];
        }
    }

    FrameBufferAttachment attachments[MAX_FRAMEBUFFER_ATTACHMENTS];
    uint32_t num;
};

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
        , handle(MIREN_INVALID_HANDLE) {}
    uint8_t *data;
    uint32_t size;
    uint32_t startVertex;
    VertexBufferHandle handle;
};

struct Size {
    int width;
    int height;

    Size()
        : width(0)
        , height(0) {}

    Size(int width, int height)
        : width(width)
        , height(height) {}

    inline bool isZero() {
        return width == 0 && height == 0;
    }
};

struct Point {
    int x;
    int y;

    Point()
        : x(0)
        , y(0) {}

    Point(int x, int y)
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

    Rect(int x, int y, int width, int height)
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
