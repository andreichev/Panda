//
// Created by Admin on 07.02.2022.
//

#include "pch.hpp"
#include "BaseLevel.hpp"
#include "Model/Vertex.hpp"
#include "Components/CameraMove.hpp"
#include "Components/FullScreenToggle.hpp"
#include "Components/UI/RootView.hpp"
#include "Model/PerlinNoise.hpp"

#include <Panda.hpp>
#include <PandaUI/PandaUI.hpp>
#include <Miren/Miren.hpp>

struct SkyVertex {
    glm::vec3 pos;

    SkyVertex(float x, float y, float z)
        : pos(x, y, z) {}
};

class SkyComponent final : public Panda::NativeScript {
public:
    void initialize() override {
        using namespace Miren;
        auto vertices = new SkyVertex[24]{
            // Front
            SkyVertex(-1.0f, -1.0f, 1.0f), // 0
            SkyVertex(1.0f, -1.0f, 1.0f),  // 1
            SkyVertex(1.0f, 1.0f, 1.0f),   // 2
            SkyVertex(-1.0f, 1.0f, 1.0f),  // 3
            // Back
            SkyVertex(-1.0f, -1.0f, -1.0f), // 4
            SkyVertex(-1.0f, 1.0f, -1.0f),  // 5
            SkyVertex(1.0f, 1.0f, -1.0f),   // 6
            SkyVertex(1.0f, -1.0f, -1.0f),  // 7
            // Top
            SkyVertex(-1.0f, 1.0f, -1.0f), // 8
            SkyVertex(-1.0f, 1.0f, 1.0f),  // 11
            SkyVertex(1.0f, 1.0f, 1.0f),   // 10
            SkyVertex(1.0f, 1.0f, -1.0f),  // 9
            // Bottom
            SkyVertex(-1.0f, -1.0f, -1.0f), // 12
            SkyVertex(1.0f, -1.0f, -1.0f),  // 13
            SkyVertex(1.0f, -1.0f, 1.0f),   // 14
            SkyVertex(-1.0f, -1.0f, 1.0f),  // 15
            // Left
            SkyVertex(-1.0f, -1.0f, -1.0f), // 16
            SkyVertex(-1.0f, -1.0f, 1.0f),  // 17
            SkyVertex(-1.0f, 1.0f, 1.0f),   // 18
            SkyVertex(-1.0f, 1.0f, -1.0f),  // 19
            // Right
            SkyVertex(1.0f, -1.0f, -1.0f), // 20
            SkyVertex(1.0f, 1.0f, -1.0f),  // 23
            SkyVertex(1.0f, 1.0f, 1.0f),   // 22
            SkyVertex(1.0f, -1.0f, 1.0f)   // 21
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
        Miren::VertexLayoutHandle layoutHandle = Miren::createVertexLayout(layoutData);
        vertexBuffer = Miren::createVertexBuffer(vertices, 24 * sizeof(Vertex), layoutHandle);
        indexBuffer = Miren::createIndexBuffer(indices, Miren::BufferElementType::UnsignedInt, 36);

        Panda::TextureAsset skyTextureAsset = Panda::AssetLoader::loadCubeMapTexture({
            "textures/skybox/px.png",
            "textures/skybox/nx.png",
            "textures/skybox/py.png",
            "textures/skybox/ny.png",
            "textures/skybox/pz.png",
            "textures/skybox/nz.png",
        });
        skyTexture = Miren::createTexture(skyTextureAsset.getMirenTextureCreate());

        Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
            "shaders/sky/sky_vertex.glsl", "shaders/sky/sky_fragment.glsl"
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
        // projection = glm::perspective(90.f, 1.0f, 0.1f, 1000.0f);
        model = glm::mat4(1.f);
        translate = glm::vec3(0.f, 0.f, 0.f);
        viewProjection = projection * view;

        Miren::setUniform(
            shader, "projViewMtx", &viewProjection[0][0], Miren::UniformDataType::Mat4
        );
        m_time = 0;
    }

    void update(double deltaTime) override {
        m_time += deltaTime;
        model = glm::mat4(1.f);
        model = glm::scale(
            glm::mat4(1.f), glm::vec3(abs(sin(m_time)) + 1.f, abs(sin(m_time)) + 1.f, 1.f)
        );
        model = glm::translate(model, translate);
        model = glm::rotate(model, (float)m_time, glm::vec3(1.f, 1.f, 0.f));
        viewProjection = projection * view;

        Miren::setShader(shader);
        Miren::setTexture(skyTexture, 0);
        Miren::setUniform(shader, "model", &model[0][0], Miren::UniformDataType::Mat4);
        Miren::setUniform(
            shader, "projViewMtx", &viewProjection[0][0], Miren::UniformDataType::Mat4
        );

        Miren::setVertexBuffer(vertexBuffer);
        Miren::setIndexBuffer(indexBuffer, 0, 36);
        Miren::submit(0);
    }

private:
    glm::vec3 translate;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 projection;
    glm::mat4 viewProjection;

    float m_time;
    Miren::TextureHandle skyTexture;
    Miren::ProgramHandle shader;
    Miren::IndexBufferHandle indexBuffer;
    Miren::VertexBufferHandle vertexBuffer;
};

class CameraSizeObserver final : public Panda::NativeScript, Panda::WindowSizeObserver {
public:
    void initialize() override {
        Panda::Application::get()->addWindowSizeObserver(this);
    }

    ~CameraSizeObserver() {
        Panda::Application::get()->removeWindowSizeObserver(this);
    }

    void update(double deltaTime) override {}

    void windowSizeChanged(Panda::Size size) override {
        m_camera->setViewportSize(size);
        PandaUI::Context::shared().updateViewportSize({size.width, size.height});
    }

    void setCamera(Panda::Camera *camera) {
        m_camera = camera;
    }

private:
    Panda::Camera *m_camera;
};

void BaseLevel::start(Panda::World *world) {
    Miren::setViewClear(0, 0x3D75C9FF);
    Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
        "shaders/ground/ground_vertex.glsl", "shaders/ground/ground_fragment.glsl"
    );
    m_shader = Miren::createProgram(programAsset.getMirenProgramCreate());
    LOG_INFO("MESH GENERATION STARTED");

    Panda::TextureAsset colorTextureAsset = Panda::AssetLoader::loadTexture("textures/grass1.png");
    Miren::TextureCreate colorTextureCreate = colorTextureAsset.getMirenTextureCreate();
    colorTextureCreate.m_numMips = 4;
    colorTextureCreate.m_minFiltering = Miren::NEAREST_MIPMAP_LINEAR;
    m_colorTexture = Miren::createTexture(colorTextureCreate);

    Panda::Entity cameraEntity = world->instantiateEntity();
    Panda::WorldCamera &camera = cameraEntity.addComponent<Panda::CameraComponent>().camera;
    camera.setFieldOfView(60.f);
    cameraEntity.addNativeScript<CameraMove>();
    CameraSizeObserver &cameraSizeObserver = cameraEntity.addNativeScript<CameraSizeObserver>();
    cameraSizeObserver.setCamera(&camera);

    cameraEntity.addNativeScript<FullScreenToggle>();

    Panda::Entity terrainEntity = world->instantiateEntity();
    Panda::DynamicMeshComponent &meshComponent =
        terrainEntity.getComponent<Panda::DynamicMeshComponent>();
    Panda::DynamicMesh &dynamicMesh = meshComponent.meshes.emplace_back();

    terrainEntity.addNativeScript<SkyComponent>();

    Panda::TextureAsset heightTextureAsset = Panda::AssetLoader::loadTexture("textures/map2.png");
    int width = heightTextureAsset.m_width;
    int height = heightTextureAsset.m_height;
    float *heightMap = (float *)ALLOC(Foundation::getAllocator(), sizeof(float) * width * height);

    uint8_t *data = (uint8_t *)heightTextureAsset.m_data.data;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            int index = h * width + w;
            heightMap[index] = data[index * 4] / 255.f;
        }
    }
    // PerlinNoise::generate2DGlm(123, 4, 1.0f, heightMap, width, height);

    Miren::VertexLayoutHandle layoutHandle =
        Miren::createVertexLayout(Vertex::createBufferLayout());
    Panda::MeshData meshData = m_meshGenerator.makeMesh(layoutHandle, width, height, heightMap);
    FREE(Foundation::getAllocator(), heightMap);

    dynamicMesh.create(meshData, m_colorTexture, m_shader);

    PandaUI::initialize();
    Foundation::Shared<RootView> view = PandaUI::makeView<RootView>();
    PandaUI::Context::shared().setRootView(view);

    LOG_INFO("BASE LEVEL STARTED!");
}

BaseLevel::~BaseLevel() {
    Miren::deleteProgram(m_shader);
    Miren::deleteTexture(m_colorTexture);
}
