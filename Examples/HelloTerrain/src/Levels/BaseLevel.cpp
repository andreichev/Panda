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

    Panda::TextureAsset heightTextureAsset = Panda::AssetLoader::loadTexture("textures/map2.png");
    int width = heightTextureAsset.m_width;
    int height = heightTextureAsset.m_height;
    float *heightMap = (float *)ALLOC(Foundation::getAllocator(), sizeof(float) * width * height);

    Panda::TextureAsset skyTextureAsset = Panda::AssetLoader::loadCubeMapTexture(
        {"textures/skybox/back.jpg",
         "textures/skybox/bottom.jpg",
         "textures/skybox/front.jpg",
         "textures/skybox/left.jpg",
         "textures/skybox/right.jpg",
         "textures/skybox/top.jpg"}
    );
    m_skyTexture = Miren::createTexture(skyTextureAsset.getMirenTextureCreate());

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
