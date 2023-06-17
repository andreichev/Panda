#pragma once

#include "Panda/Base/Base.hpp"
#include "Panda/GameLogic/Components/OrthographicCamera.hpp"
#include "Panda/Renderer/Texture.hpp"

#include <Miren/Miren.hpp>

namespace Panda {

#define MAX_VERTICES_COUNT 10000
#define MAX_INDICES_COUNT 16000
#define MAX_TEXTURE_SLOTS 8

class Renderer2D {
public:
    struct RectData {
        RectData()
            : origin()
            , size()
            , color()
            , texture(nullptr)
            , rotation(0) {}

        Point origin;
        Size size;
        Color color;
        float rotation;
        Foundation::Shared<Texture> texture;
    };

    struct Statistics {
        uint32_t drawCalls = 0;
        uint32_t quadCount = 0;

        uint32_t getTotalVertexCount() const {
            return quadCount * 4;
        }
        uint32_t getTotalIndexCount() const {
            return quadCount * 6;
        }
    };

    static void init();
    static void begin();
    static void drawRect(RectData rect);
    static void end();
    static void resetStats();
    static Statistics getStats();
    static void terminate();
    static void setViewId(Miren::ViewId id);
    static void setCamera(Foundation::Shared<OrthographicCamera> camera);

private:
    static Miren::ViewId s_viewId;
    static Foundation::Shared<OrthographicCamera> s_camera;
    static void drawRect(glm::mat4 &transform, RectData rect);
};

} // namespace Panda