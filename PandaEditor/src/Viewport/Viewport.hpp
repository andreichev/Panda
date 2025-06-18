//
// Created by Andreichev Mikhail on 08.06.2025.
//

#pragma once

#include <Panda/Base/Base.hpp>
#include <Miren/Miren.hpp>
#include <unordered_set>

namespace Panda {

class Viewport {
public:
    Viewport();
    ~Viewport();
    void initWithSize(Vec2 size);
    void updateSize(Size size);
    void updateOrigin(Vec2 origin);
    Miren::TextureHandle getResultTexture();
    Miren::ViewId getRenderingView();
    std::unordered_set<UUID> getEntitiesInsideRect(Rect rect);
    UUID getEntityInsidePoint(Vec2 point);
    Rect getFrame();
    void drawOutline(const std::unordered_set<UUID> &selection);
    void readIdsBuffer();

private:
    Rect m_frame;
    // SCENE RENDERING:
    Miren::FrameBufferHandle m_sceneFB;
    Miren::FrameBufferSpecification m_sceneFbSpecification;
    Miren::ViewId m_sceneView;
    Miren::TextureHandle m_colorAttachment;
    Foundation::Memory m_idsBuffer;

    // SELECTED OBJECT HIGHLIGHT OUTLINE:
    Miren::VertexBufferHandle m_vertexBuffer;
    Miren::IndexBufferHandle m_indexBuffer;
    Miren::ProgramHandle m_outlineProgram;
    Miren::FrameBufferHandle m_outputFB;
    Miren::TextureHandle m_resultAttachment;
    Miren::FrameBufferSpecification m_outputFbSpecification;
    Miren::ViewId m_outputView;
};

} // namespace Panda
