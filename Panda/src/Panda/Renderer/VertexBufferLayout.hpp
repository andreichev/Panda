//
// Created by Admin on 11.02.2022.
//

#pragma once

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

class VertexBufferLayout {
public:
    VertexBufferLayout()
        : m_Stride(0) {}

    virtual ~VertexBufferLayout() = default;

    template<typename T>
    void push(unsigned int count) {
        PND_ERROR("UNDEFINED REALIZATION");
    }

    template<>
    void push<float>(unsigned int count) {
        VertexBufferElement element = {BufferElementType::Float, count, false};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::Float);
    }

    template<>
    void push<unsigned int>(unsigned int count) {
        VertexBufferElement element = {BufferElementType::UnsignedInt, count, false};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::UnsignedInt);
    }

    template<>
    void push<char>(unsigned int count) {
        VertexBufferElement element = {BufferElementType::UnsignedByte, count, true};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::UnsignedByte);
    }

    template<>
    void push<glm::vec3>(unsigned int count) {
        VertexBufferElement element = {BufferElementType::Float, count * 3, true};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::Float) * 3;
    }

    template<>
    void push<glm::vec2>(unsigned int count) {
        VertexBufferElement element = {BufferElementType::Float, count * 2, false};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(BufferElementType::Float) * 2;
    }

    void pushVector() {
        // positions
        push<glm::vec3>(1);
        // texture coordinates
        push<glm::vec2>(1);
        // light
        push<float>(1);
    }

    virtual void initializeForRenderer() = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;

protected:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};
} // namespace Panda
