//
// Created by Admin on 11.02.2022.
//

#pragma once

#include <Foundation/Assert.hpp>
#include <Foundation/Logger.hpp>

namespace Miren {

#define MAX_VERTEX_LAYOUT_ELEMENTS 20

enum class BufferElementType { Float, UnsignedInt, UnsignedShort, UnsignedByte };

struct VertexBufferElement {
    BufferElementType type;
    uint32_t count;
    bool normalized;

    static size_t getSizeOfType(BufferElementType type) {
        switch (type) {
            case BufferElementType::Float:
                return 4;
            case BufferElementType::UnsignedInt:
                return 4;
            case BufferElementType::UnsignedShort:
                return 2;
            case BufferElementType::UnsignedByte:
                return 1;
        }
        ASSERT(false, "Vertex buffer element type is undefined");
        return 0;
    }
};

class VertexBufferLayoutData {
public:
    VertexBufferLayoutData(const VertexBufferLayoutData &other)
        : m_Stride(other.m_Stride)
        , m_ElementsCount(other.m_ElementsCount) {
        for (size_t i = 0; i < m_ElementsCount; i++) {
            m_Elements[i] = other.m_Elements[i];
        }
    }

    VertexBufferLayoutData(const VertexBufferLayoutData &&other) = delete;

    VertexBufferLayoutData()
        : m_Stride(0)
        , m_ElementsCount(0) {}

    virtual ~VertexBufferLayoutData() = default;

    void pushFloat(uint32_t count) {
        VertexBufferElement element = {BufferElementType::Float, count, false};
        m_Elements[m_ElementsCount++] = element;
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::Float);
    }

    void pushUInt(uint32_t count) {
        VertexBufferElement element = {BufferElementType::UnsignedInt, count, false};
        m_Elements[m_ElementsCount++] = element;
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::UnsignedInt);
    }

    void pushChar(uint32_t count, bool normalized) {
        VertexBufferElement element = {BufferElementType::UnsignedByte, count, normalized};
        m_Elements[m_ElementsCount++] = element;
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::UnsignedByte);
    }

    void pushVec3() {
        VertexBufferElement element = {BufferElementType::Float, 3, false};
        m_Elements[m_ElementsCount++] = element;
        m_Stride += VertexBufferElement::getSizeOfType(BufferElementType::Float) * 3;
    }

    void pushVec4() {
        VertexBufferElement element = {BufferElementType::Float, 4, false};
        m_Elements[m_ElementsCount++] = element;
        m_Stride += VertexBufferElement::getSizeOfType(BufferElementType::Float) * 4;
    }

    void push8BitRGBAColor() {
        VertexBufferElement element = {BufferElementType::UnsignedByte, 4, true};
        m_Elements[m_ElementsCount++] = element;
        m_Stride += VertexBufferElement::getSizeOfType(BufferElementType::UnsignedByte) * 4;
    }

    void pushVec2() {
        VertexBufferElement element = {BufferElementType::Float, 2, false};
        m_Elements[m_ElementsCount++] = element;
        m_Stride += VertexBufferElement::getSizeOfType(BufferElementType::Float) * 2;
    }

    void pushVector() {
        // positions
        pushVec3();
        // texture coordinates
        pushVec2();
        // light
        pushFloat(1);
    }

    VertexBufferElement m_Elements[MAX_VERTEX_LAYOUT_ELEMENTS];
    uint32_t m_ElementsCount;
    uint32_t m_Stride;
};

} // namespace Miren
