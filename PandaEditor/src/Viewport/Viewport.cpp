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

    // -----------------------------------------------------------------------
    //             SCENE RENDERING FRAMEBUFFER AND ATTACHMENTS
    // -----------------------------------------------------------------------

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

    // -----------------------------------------------------------------------
    // SELECTED OBJECT HIGHLIGHT OUTLINE RENDERING FRAMEBUFFER AND ATTACHMENTS
    // -----------------------------------------------------------------------

    create.m_format = Miren::TextureFormat::RGBA8;
    m_resultAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment outputAttachments[] = {m_resultAttachment};
    m_outputFbSpecification = Miren::FrameBufferSpecification(outputAttachments, 1);
    m_outputFB = Miren::createFrameBuffer(m_outputFbSpecification);
    Miren::setViewport(
        m_outputView,
        Miren::Rect(0, 0, m_frame.size.width * dpi.width, m_frame.size.height * dpi.height)
    );
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
    m_idsBuffer.release();
    uint32_t bufferSize =
        sizeof(uint32_t) * m_frame.size.width * dpi.width * m_frame.size.height * dpi.height;
    m_idsBuffer = Foundation::Memory::alloc(bufferSize);
    memset(m_idsBuffer.data, 0, bufferSize);
}

Miren::TextureHandle Viewport::getResultTexture() {
    return m_colorAttachment;
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

void Viewport::viewportDrawOutline() {}

void Viewport::viewportReadIdsBuffer() {
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