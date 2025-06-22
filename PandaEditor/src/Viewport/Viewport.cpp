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
    , m_outputFbSpecification()
    , m_selectedGeometryFB()
    , m_selectedAttachment()
    , m_selectedFbSpecification()
    , m_selectedGeometryView(3) {}

Viewport::~Viewport() {
    Miren::deleteFrameBuffer(m_sceneFB);
    for (int i = 0; i < m_sceneFbSpecification.num; i++) {
        Miren::FrameBufferAttachment attachment = m_sceneFbSpecification.attachments[i];
        Miren::deleteTexture(attachment.handle);
    }
    Miren::deleteFrameBuffer(m_selectedGeometryFB);
    for (int i = 0; i < m_selectedFbSpecification.num; i++) {
        Miren::FrameBufferAttachment attachment = m_selectedFbSpecification.attachments[i];
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
    uint32_t bufferSize =
        sizeof(uint32_t) * m_frame.size.width * dpi.width * m_frame.size.height * dpi.height;
    m_idsBuffer = Foundation::Memory::alloc(bufferSize);
    memset(m_idsBuffer.data, 0, bufferSize);

    // -------------------------------------------
    //              SCENE RENDERING
    // -------------------------------------------

    Miren::TextureCreate create;
    create.m_minFiltering = Miren::NEAREST;
    create.m_magFiltering = Miren::NEAREST;
    create.m_wrap = Miren::CLAMP;
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
    Miren::setViewClearAttachments(m_sceneView, {Miren::Clear(1, 0), Miren::Clear(2, 0)});

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
    m_resultAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment outputAttachments[] = {m_resultAttachment};
    m_outputFbSpecification = Miren::FrameBufferSpecification(outputAttachments, 1);
    m_outputFB = Miren::createFrameBuffer(m_outputFbSpecification);

    Miren::setViewport(
        m_outputView,
        Miren::Rect(0, 0, m_frame.size.width * dpi.width, m_frame.size.height * dpi.height)
    );
    Miren::setViewClear(m_outputView, 0x000000ff);
    Miren::setViewFrameBuffer(m_outputView, m_outputFB);

    // -------------------------------------------
    //  SELECTED GEOMETRY ISOLATION FRAME BUFFER
    // -------------------------------------------

    create.m_width = m_frame.size.width;
    create.m_height = m_frame.size.height;
    create.m_format = Miren::TextureFormat::R8UI;
    m_selectedAttachment = Miren::createTexture(create);
    create.m_format = Miren::TextureFormat::DEPTH24STENCIL8;
    depthAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment selectedGeometryFBAttachments[] = {
        m_selectedAttachment, depthAttachment
    };
    m_selectedFbSpecification = Miren::FrameBufferSpecification(selectedGeometryFBAttachments, 2);
    m_selectedGeometryFB = Miren::createFrameBuffer(m_selectedFbSpecification);
    Miren::setViewport(
        m_selectedGeometryView, Miren::Rect(0, 0, m_frame.size.width, m_frame.size.height)
    );
    Miren::setViewClear(m_selectedGeometryView, 0);
    Miren::setViewFrameBuffer(m_selectedGeometryView, m_selectedGeometryFB);
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
    // IDS BUFFER
    uint32_t bufferSize =
        sizeof(uint32_t) * m_frame.size.width * dpi.width * m_frame.size.height * dpi.height;
    m_idsBuffer.release();
    m_idsBuffer = Foundation::Memory::alloc(bufferSize);
    memset(m_idsBuffer.data, 0, bufferSize);
    // SELECTED GEOMETRY FRAME BUFFER
    Miren::setViewport(m_selectedGeometryView, Miren::Rect(0, 0, size.width, size.height));
    // IS SELECTED ATTACHMENT
    Miren::resizeTexture(m_selectedFbSpecification.attachments[0].handle, size.width, size.height);
    // DEPTH ATTACHMENT
    Miren::resizeTexture(m_selectedFbSpecification.attachments[1].handle, size.width, size.height);
    Miren::deleteFrameBuffer(m_selectedGeometryFB);
    m_selectedGeometryFB = Miren::createFrameBuffer(m_selectedFbSpecification);
    Miren::setViewFrameBuffer(m_selectedGeometryView, m_selectedGeometryFB);
}

Miren::TextureHandle Viewport::getResultTexture() {
    return m_resultAttachment;
}

Miren::ViewId Viewport::getRenderingView() {
    return m_sceneView;
}

Miren::ViewId Viewport::getSelectionRenderingView() {
    return m_selectedGeometryView;
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

void Viewport::drawOutline(float dt, const std::unordered_set<UUID> &selection) {
    // RENDERING SELECTION HIGHLIGHT
    Miren::setState(0);
    Miren::setShader(m_outlineProgram);
    int samplerColor = 0;
    Miren::setTexture(m_colorAttachment, samplerColor);
    Miren::setUniform(m_outlineProgram, "colorTexture", &samplerColor, Miren::UniformType::Sampler);
    int samplerIsSelected = 1;
    Miren::setTexture(m_selectedAttachment, samplerIsSelected);
    Miren::setUniform(
        m_outlineProgram, "isSelectedTexture", &samplerIsSelected, Miren::UniformType::Sampler
    );
    Fern::Size dpi = Application::get()->getMainWindow()->getDpi();
    Vec2 size = m_frame.size;
    Miren::setUniform(m_outlineProgram, "resolution", &size, Miren::UniformType::Vec2);
    static float time = 0.f;
    time += dt;
    Miren::setUniform(m_outlineProgram, "time", &time, Miren::UniformType::Float);
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