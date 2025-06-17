//
// Created by Andreichev Mikhail on 08.06.2025.
//

#include "Viewport.hpp"

#include <Panda/Application/Application.hpp>
#include <PandaUI/PandaUI.hpp>

namespace Panda {

Viewport::Viewport()
    : m_frame()
    , m_idsBuffer()
    , m_sceneFB()
    , m_sceneFbSpecification()
    , m_sceneView(1)
    , m_colorAttachment()
    , m_outlineProgram()
    , m_outputFB()
    , m_resultAttachment()
    , m_outputView(2)
    , m_outputFbSpecification() {}

Viewport::~Viewport() {
    Miren::deleteFrameBuffer(m_sceneFB);
    for (int i = 0; i < m_sceneFbSpecification.num; i++) {
        Miren::FrameBufferAttachment attachment = m_sceneFbSpecification.attachments[i];
        Miren::deleteTexture(attachment.handle);
    }
    Miren::deleteFrameBuffer(m_outputFB);
    for (int i = 0; i < m_outputFbSpecification.num; i++) {
        Miren::FrameBufferAttachment attachment = m_outputFbSpecification.attachments[i];
        Miren::deleteTexture(attachment.handle);
    }
    Miren::deleteProgram(m_outlineProgram);
}

void Viewport::initWithSize(Vec2 size) {
    m_frame.size = size;
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    PandaUI::initialize();
    PandaUI::Context::shared().updateViewId(m_sceneView);
    uint32_t bufferSize = sizeof(uint32_t) * m_frame.size.width / 4 * m_frame.size.height / 4;
    m_idsBuffer = Foundation::Memory::alloc(bufferSize);
    memset(m_idsBuffer.data, 0, bufferSize);

    // -------------------------------------------
    //              SCENE RENDERING
    // -------------------------------------------

    Miren::TextureCreate create;
    create.m_format = Miren::TextureFormat::RGBA8;
    create.m_width = m_frame.size.width * dpi.width;
    create.m_height = m_frame.size.height * dpi.height;
    m_colorAttachment = Miren::createTexture(create);
    create.m_format = Miren::TextureFormat::R32UI;
    Miren::TextureHandle idAttachment = Miren::createTexture(create);
    create.m_format = Miren::TextureFormat::DEPTH24STENCIL8;
    Miren::TextureHandle depthAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment sceneAttachments[] = {
        m_colorAttachment, idAttachment, depthAttachment
    };
    m_sceneFbSpecification = Miren::FrameBufferSpecification(sceneAttachments, 3);
    m_sceneFB = Miren::createFrameBuffer(m_sceneFbSpecification);
    Miren::setViewport(
        m_sceneView,
        Miren::Rect(0, 0, m_frame.size.width * dpi.width, m_frame.size.height * dpi.height)
    );
    Miren::setViewFrameBuffer(m_sceneView, m_sceneFB);
    Miren::setViewClear(m_sceneView, 0x000000cff);
    Miren::setViewClearAttachments(m_sceneView, {Miren::Clear(1, 0)});

    // -------------------------------------------
    // SELECTED OBJECT HIGHLIGHT OUTLINE RENDERING
    // -------------------------------------------

    Miren::VertexBufferLayoutData layoutData;
    // Position
    layoutData.pushVec3();
    // Texture coordinates
    layoutData.pushVec2();
    Miren::VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);

    // clang-format off
    float x = -1.f, y = -1.f, z = 0.f;
    auto *vertices = new float[20] {
        // x     y        z  u  v
        x,       y,       z, 0, 0,
        x + 2.f, y,       z, 1, 0,
        x + 2.f, y + 2.f, z, 1, 1,
        x,       y + 2.f, z, 0, 1
    };
    // clang-format on

    auto *indices = new uint32_t[6]{0, 1, 2, 2, 3, 0};
    m_vertexBuffer = createVertexBuffer(vertices, sizeof(float) * 20, vertexLayout);
    m_indexBuffer = Miren::createIndexBuffer(indices, Miren::BufferElementType::UnsignedInt, 6);

    Foundation::Memory vertexMem =
        AssetImporterBase::loadData("editor-shaders/selection_highlight_vertex.glsl");
    Foundation::Memory fragmentMem =
        AssetImporterBase::loadData("editor-shaders/selection_highlight_fragment.glsl");
    m_outlineProgram = Miren::createProgram({vertexMem, fragmentMem});

    create.m_format = Miren::TextureFormat::RGBA8;
    create.m_minFiltering = Miren::NEAREST;
    create.m_magFiltering = Miren::NEAREST;
    create.m_wrap = Miren::CLAMP;
    m_resultAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment outputAttachments[] = {m_resultAttachment};
    m_outputFbSpecification = Miren::FrameBufferSpecification(outputAttachments, 1);
    m_outputFB = Miren::createFrameBuffer(m_outputFbSpecification);

    create.m_format = Miren::TextureFormat::R8;
    m_highlightMapTexture = Miren::createTexture(create);

    Miren::setViewport(
        m_outputView,
        Miren::Rect(0, 0, m_frame.size.width * dpi.width, m_frame.size.height * dpi.height)
    );
    Miren::setViewClear(m_outputView, 0x000000ff);
    Miren::setViewFrameBuffer(m_outputView, m_outputFB);
}

void Viewport::updateOrigin(Vec2 origin) {
    m_frame.origin = origin;
}

void Viewport::updateSize(Size size) {
    m_frame.size = size;
    PandaUI::Context::shared().updateViewportSize({size.width, size.height});
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    Miren::setViewport(
        m_sceneView, Miren::Rect(0, 0, size.width * dpi.width, size.height * dpi.height)
    );
    // COLOR ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[0].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    // ID ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[1].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    // DEPTH ATTACHMENT
    Miren::resizeTexture(
        m_sceneFbSpecification.attachments[2].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    Miren::deleteFrameBuffer(m_sceneFB);
    m_sceneFB = Miren::createFrameBuffer(m_sceneFbSpecification);
    Miren::setViewFrameBuffer(m_sceneView, m_sceneFB);
    // HIGHLIGHT FRAME BUFFER
    Miren::setViewport(
        m_outputView, Miren::Rect(0, 0, size.width * dpi.width, size.height * dpi.height)
    );
    Miren::resizeTexture(
        m_outputFbSpecification.attachments[0].handle,
        size.width * dpi.width,
        size.height * dpi.height
    );
    Miren::deleteFrameBuffer(m_outputFB);
    m_outputFB = Miren::createFrameBuffer(m_outputFbSpecification);
    Miren::setViewFrameBuffer(m_outputView, m_outputFB);
    // HIGHLIGHT TEXTURE
    Miren::resizeTexture(m_highlightMapTexture, size.width / 4, size.height / 4);
    uint32_t bufferSize = sizeof(uint32_t) * m_frame.size.width / 4 * m_frame.size.height / 4;
    m_idsBuffer.release();
    m_idsBuffer = Foundation::Memory::alloc(bufferSize);
    memset(m_idsBuffer.data, 0, bufferSize);
}

Miren::TextureHandle Viewport::getResultTexture() {
    return m_resultAttachment;
}

Miren::ViewId Viewport::getRenderingView() {
    return m_sceneView;
}

std::unordered_set<UUID> Viewport::getEntitiesInsideRect(Rect rect) {
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    //      m_frame
    //   _________________________
    //   |      selectedRegion   |
    //   |      ___________      |
    //   |     |           |     |
    //   |     |___________|     |
    //   |_______________________|
    //
    Rect selectedRegion = rect;
    selectedRegion.size.x *= dpi.x;
    selectedRegion.size.y *= dpi.y;
    selectedRegion.origin.x *= dpi.x;
    selectedRegion.origin.y *= dpi.y;
    std::unordered_set<UUID> ids;
    if (selectedRegion.size.width < 0) {
        selectedRegion.origin.x += selectedRegion.size.width;
        selectedRegion.size.width = -selectedRegion.size.width;
    }
    if (selectedRegion.size.height < 0) {
        selectedRegion.origin.y += selectedRegion.size.height;
        selectedRegion.size.height = -selectedRegion.size.height;
    }
    for (int regX = 0; regX < selectedRegion.size.width; regX++) {
        for (int regY = 0; regY < selectedRegion.size.height; regY++) {
            int texelX = selectedRegion.origin.x + regX;
            int texelY = m_frame.size.height * dpi.y - selectedRegion.origin.y - regY;
            uint32_t texelIndex = m_frame.size.width * dpi.x * texelY + texelX;
            uint32_t *buffer = static_cast<uint32_t *>(m_idsBuffer.data);
            uint32_t hoveredId = buffer[texelIndex];
            if (hoveredId != 0) { ids.insert(hoveredId); }
        }
    }
    return ids;
}

UUID Viewport::getEntityInsidePoint(Vec2 point) {
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    int texelX = point.x * dpi.x;
    int texelY = m_frame.size.height * dpi.y - point.y * dpi.y;
    uint32_t texelIndex = m_frame.size.width * dpi.x * texelY + texelX;
    uint32_t *buffer = static_cast<uint32_t *>(m_idsBuffer.data);
    uint32_t hoveredId = buffer[texelIndex];
    return hoveredId;
}

void Viewport::drawOutline(const std::unordered_set<UUID> &selection) {
    // UPDATING HIGHLIGHT TEXTURE
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    uint32_t idsWidth = m_frame.size.width * dpi.x;
    uint32_t idsHeight = m_frame.size.height * dpi.y;
    uint32_t highlightWidth = m_frame.size.width / 4;
    uint32_t highlightHeight = m_frame.size.height / 4;
    uint32_t bufferSize = 4 * highlightWidth * highlightHeight;
    Foundation::Memory highlightBuffer = Foundation::Memory::alloc(bufferSize);
    uint8_t *highlightData = static_cast<uint8_t *>(highlightBuffer.data);
    uint32_t *idsData = static_cast<uint32_t *>(m_idsBuffer.data);
    for (int y = 0; y < highlightWidth; y++) {
        for (int x = 0; x < highlightHeight; x++) {
            int idsX = Foundation::min(x * (float)idsWidth / highlightWidth, (float)idsWidth - 1);
            int idsY =
                Foundation::min(y * (float)idsHeight / highlightHeight, (float)idsHeight - 1);
            int texelIndexIds = idsWidth * idsY + idsX;
            int texelIndexHighlight = highlightWidth * y + x;
            UUID id = idsData[texelIndexIds];
            if (selection.contains(id)) {
                highlightData[texelIndexHighlight] = 0xff;
            } else {
                highlightData[texelIndexHighlight] = 0x00;
            }
        }
    }
    Miren::updateTexture(m_highlightMapTexture, highlightBuffer);
    // RENDERING SELECTION HIGHLIGHT
    Miren::setState(0);
    Miren::setShader(m_outlineProgram);
    int samplerColor = 0;
    Miren::setTexture(m_colorAttachment, samplerColor);
    Miren::setUniform(m_outlineProgram, "colorTexture", &samplerColor, Miren::UniformType::Sampler);
    int samplerHighlight = 1;
    Miren::setTexture(m_highlightMapTexture, samplerHighlight);
    Miren::setUniform(
        m_outlineProgram, "highlightMapTexture", &samplerHighlight, Miren::UniformType::Sampler
    );
    Miren::setVertexBuffer(m_vertexBuffer);
    Miren::setIndexBuffer(m_indexBuffer, 0, 6);
    Miren::submit(m_outputView);
}

void Viewport::readIdsBuffer() {
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    Miren::readFrameBuffer(
        m_sceneFB,
        1,
        0,
        0,
        m_frame.size.width * dpi.x,
        m_frame.size.height * dpi.y,
        m_idsBuffer.data
    );
}

Rect Viewport::getFrame() {
    return m_frame;
}

} // namespace Panda