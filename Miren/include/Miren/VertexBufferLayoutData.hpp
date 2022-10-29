//
// Created by Admin on 11.02.2022.
//

#pragma once

#include <Foundation/Logger.hpp>

namespace Panda {

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
        PND_ERROR("Vertex buffer element type is undefined");
        return 0;
    }
};

class VertexBufferLayoutData {
public:
    VertexBufferLayoutData()
        : m_Stride(0) {}

    virtual ~VertexBufferLayoutData() = default;

    void pushFloat(unsigned int count) {
        VertexBufferElement element = {BufferElementType::Float, count, false};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::Float);
    }

    void pushUInt(unsigned int count) {
        VertexBufferElement element = {BufferElementType::UnsignedInt, count, false};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::UnsignedInt);
    }

    void pushChar(unsigned int count, bool normalized) {
        VertexBufferElement element = {BufferElementType::UnsignedByte, count, normalized};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::UnsignedByte);
    }

    void pushVec3() {
        VertexBufferElement element = {BufferElementType::Float, 3, false};
        m_Elements.push_back(element);
        m_Stride += VertexBufferElement::getSizeOfType(BufferElementType::Float) * 3;
    }

    void pushVec4() {
        VertexBufferElement element = {BufferElementType::Float, 4, false};
        m_Elements.push_back(element);
        m_Stride += VertexBufferElement::getSizeOfType(BufferElementType::Float) * 4;
    }

    void push8BitRGBAColor() {
        VertexBufferElement element = {BufferElementType::UnsignedByte, 4, true};
        m_Elements.push_back(element);
        m_Stride += VertexBufferElement::getSizeOfType(BufferElementType::UnsignedByte) * 4;
    }

    void pushVec2() {
        VertexBufferElement element = {BufferElementType::Float, 2, false};
        m_Elements.push_back(element);
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

    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};

} // namespace Panda
