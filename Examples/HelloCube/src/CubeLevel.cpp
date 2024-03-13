//
// Created by Admin on 06/04/2022.
//

#include "CubeLevel.hpp"

#include <imgui.h>

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    float light;

    Vertex(float x, float y, float z, float u, float v, float light)
        : pos(x, y, z)
        , uv(u, v)
        , light(light) {}
};

class CubeComponent final : public Panda::NativeScript {
public:
    void initialize() override {
        using namespace Miren;
        auto vertices = new Vertex[24]{
            // Front
            Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f), // 0
            Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f),  // 1
            Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 2
            Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),  // 3
            // Back
            Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 4
            Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f),  // 5
            Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f),   // 6
            Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f),  // 7
            // Top
            Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 8
            Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),  // 11
            Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 10
            Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f),  // 9
            // Bottom
            Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 12
            Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f),  // 13
            Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 14
            Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),  // 15
            // Left
            Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 16
            Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f),  // 17
            Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 18
            Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f),  // 19
            // Right
            Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f), // 20
            Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f),  // 23
            Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f),   // 22
            Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f)   // 21
        };

        // clang-format off
        auto indices = new uint32_t[36]{
            0, 1, 2, 2, 3, 0,       // Front
            4, 5, 6, 6, 7, 4,       // Back
            8, 9, 10, 10, 11, 8,    // Top
            12, 13, 14, 14, 15, 12, // Bottom
            16, 17, 18, 18, 19, 16, // Left
            20, 21, 22, 22, 23, 20  // Right
        };
        // clang-format on

        Miren::VertexBufferLayoutData layoutData;
        layoutData.pushVec3();
        layoutData.pushVec2();
        layoutData.pushFloat(1);
        Miren::VertexLayoutHandle layoutHandle = Miren::createVertexLayout(layoutData);
        vertexBuffer = Miren::createVertexBuffer(vertices, 24 * sizeof(Vertex), layoutHandle);
        indexBuffer = Miren::createIndexBuffer(indices, Miren::BufferElementType::UnsignedInt, 36);

        Panda::TextureAsset textureAsset = Panda::AssetLoader::loadTexture("textures/arbuz1.png");
        texture = Miren::createTexture(textureAsset.getMirenTextureCreate());
        Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
            "default-shaders/renderer3d/base_vertex.glsl",
            "default-shaders/renderer3d/base_fragment.glsl"
        );
        shader = Miren::createProgram(programAsset.getMirenProgramCreate());
        Miren::setShader(shader);

        Panda::Size windowSize = Panda::Application::get()->getWindow()->getSize();
        view = glm::lookAt(
            glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
        );
        // view = glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective<float>(
            40.f, (float)windowSize.width / (float)windowSize.height, 0.1f, 1000.0f
        );
        // projectionMatrix = glm::perspective(90.f, 1.0f, 0.1f, 1000.0f);
        model = glm::mat4(1.f);
        translate = glm::vec3(0.f, 0.f, 0.f);
        viewProjection = projection * view;

        Miren::setUniform(
            shader, "projViewMtx", &viewProjection[0][0], Miren::UniformDataType::Mat4
        );
        time = 0;
    }

    void update(double deltaTime) override {
        time += deltaTime;
        model = glm::mat4(1.f);
        model =
            glm::scale(glm::mat4(1.f), glm::vec3(abs(sin(time)) + 1.f, abs(sin(time)) + 1.f, 1.f));
        model = glm::translate(model, translate);
        model = glm::rotate(model, (float)time, glm::vec3(1.f, 1.f, 0.f));
        viewProjection = projection * view;

        Miren::setShader(shader);
        Miren::setTexture(texture, 0);
        Miren::setUniform(shader, "model", &model[0][0], Miren::UniformDataType::Mat4);
        Miren::setUniform(
            shader, "projViewMtx", &viewProjection[0][0], Miren::UniformDataType::Mat4
        );

        Miren::setVertexBuffer(vertexBuffer);
        Miren::setIndexBuffer(indexBuffer, 0, 36);
        Miren::submit(0);
    }

    void onImGuiRender() override {
        ImGui::ShowDemoWindow();
    }

private:
    glm::vec3 translate;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 projection;
    glm::mat4 viewProjection;

    double time;
    Miren::TextureHandle texture;
    Miren::ProgramHandle shader;
    Miren::IndexBufferHandle indexBuffer;
    Miren::VertexBufferHandle vertexBuffer;
};

void CubeLevel::start(Panda::World *world) {
    LOG_INFO("CUBE LEVEL STARTED!");

    Panda::Entity entity = world->instantiateEntity();
    entity.addNativeScript<CubeComponent>();
}
