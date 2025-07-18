//
// Created by Andreichev Mikhail on 08.06.2025.
//

#include "Viewport.hpp"

#include <Panda/Application/Application.hpp>
#include <PandaUI/PandaUI.hpp>

namespace Panda {

Viewport::Viewport()
    : m_frame()
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
    , m_selectedGeometryAttachment()
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
    create.m_format = Miren::TextureFormat::RGBA8;
    m_selectedGeometryAttachment = Miren::createTexture(create);
    Miren::FrameBufferAttachment selectedGeometryFBAttachments[] = {m_selectedGeometryAttachment};
    m_selectedFbSpecification = Miren::FrameBufferSpecification(selectedGeometryFBAttachments, 1);
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
    // SELECTED GEOMETRY FRAME BUFFER
    Miren::setViewport(m_selectedGeometryView, Miren::Rect(0, 0, size.width, size.height));
    // SELECTED GEOMETRY ATTACHMENT
    Miren::resizeTexture(m_selectedFbSpecification.attachments[0].handle, size.width, size.height);
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

Miren::FrameBufferHandle Viewport::getSceneFrameBuffer() {
    return m_sceneFB;
}

void Viewport::drawOutline(float dt, const std::unordered_set<UUID> &selection) {
    // RENDERING SELECTION HIGHLIGHT
    Miren::setState(0);
    Miren::setShader(m_outlineProgram);
    int samplerColor = 0;
    Miren::setTexture(m_colorAttachment, samplerColor);
    Miren::setUniform(m_outlineProgram, "colorTexture", &samplerColor, Miren::UniformType::Sampler);
    int samplerSelectedGeometry = 1;
    Miren::setTexture(m_selectedGeometryAttachment, samplerSelectedGeometry);
    Miren::setUniform(
        m_outlineProgram,
        "selectedGeometryTexture",
        &samplerSelectedGeometry,
        Miren::UniformType::Sampler
    );
    Miren::setVertexBuffer(m_vertexBuffer);
    Miren::setIndexBuffer(m_indexBuffer, 0, 6);
    Miren::submit(m_outputView);
}

Rect Viewport::getFrame() {
    return m_frame;
}

} // namespace Panda