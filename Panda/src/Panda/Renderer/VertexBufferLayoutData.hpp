//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Panda/Logger/Logger.hpp"

namespace Panda {

enum class BufferElementType { Float, UnsignedInt, UnsignedByte };

struct VertexBufferElement {
    BufferElementType type;
    unsigned int count;
    bool normalized;

    static unsigned int getSizeOfType(BufferElementType type) {
        switch (type) {
            case BufferElementType::Float:
                return 4;
            case BufferElementType::UnsignedInt:
                return 4;
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

    void pushChar(unsigned int count) {
        VertexBufferElement element = {BufferElementType::UnsignedByte, count, true};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::UnsignedByte);
    }

    void pushVec3(unsigned int count) {
        VertexBufferElement element = {BufferElementType::Float, count * 3, true};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::Float) * 3;
    }

    void pushVec2(unsigned int count) {
        VertexBufferElement element = {BufferElementType::Float, count * 2, false};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::Float) * 2;
    }

    void pushVector() {
        // positions
        pushVec3(1);
        // texture coordinates
        pushVec2(1);
        // light
        pushFloat(1);
    }

    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};

} // namespace Panda
