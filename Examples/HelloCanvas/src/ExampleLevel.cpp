//
// Created by Admin on 19/03/2022.
//

#include "ExampleLevel.hpp"
#include "OrthographicCameraMove.hpp"

#include <Panda/Application/Application.hpp>
#include <Panda/Renderer/Renderer2D.hpp>
#include <Panda/GameLogic/Components/ParticleSystem.hpp>
#include <Panda/GameLogic/Components/OrthographicCamera.hpp>

#include <imgui.h>

class ExampleRenderer : public Panda::Component {
public:
    void initialize() override {
        Panda::Size dpi = Panda::Application::get()->getWindow()->getDpi();
        Panda::Size windowSize = Panda::Application::get()->getWindow()->getSize();
        m_texture = Foundation::makeShared<Panda::Texture>("textures/arbuz1.png");
        Miren::TextureCreate create;
        create.m_data = Foundation::Memory(nullptr);
        create.m_format = Miren::TextureFormat::RGBA8;
        create.m_width = m_sceneSize.width * dpi.width;
        create.m_height = m_sceneSize.height * dpi.height;
        colorAttachment = Miren::createTexture(create);
        create.m_format = Miren::TextureFormat::DEPTH24STENCIL8;
        Miren::TextureHandle depthAttachment = Miren::createTexture(create);
        Miren::FrameBufferAttachment attachments[] = {colorAttachment, depthAttachment};
        Miren::FrameBufferSpecification spec(attachments, 2);
        frameBuffer = Miren::createFrameBuffer(spec);
        Miren::setViewport(sceneViewId,
            Miren::Rect(0, 0, m_sceneSize.width * dpi.width, m_sceneSize.height * dpi.height));
        Miren::setViewport(
            0, Miren::Rect(0, 0, windowSize.width * dpi.width, windowSize.height * dpi.height));
        Miren::setViewClear(sceneViewId, 0x111111ff);
    }

    ~ExampleRenderer() {}

    void update(double deltaTime) override {
        degree += 60 * deltaTime;
        colorFactor += deltaTime * 0.3f;
        if (colorFactor >= 1.f) {
            colorFactor = 0.f;
        }

        Panda::Renderer2D::setFrameBuffer(frameBuffer);
        Panda::Renderer2D::setViewId(sceneViewId);

        Panda::Renderer2D::RectData rect1;
        rect1.color = Panda::Color(1.0f, 0.5f, colorFactor, 1.f);
        rect1.origin = Panda::Point(0.3f, -0.5f);
        rect1.size = Panda::Size(0.4f, 0.2f);
        rect1.rotation = degree;
        rect1.texture = m_texture;
        Panda::Renderer2D::drawRect(rect1);

        Panda::Renderer2D::RectData rect2;
        rect2.color = Panda::Color(1.0f, 0.f, 0.f, 1.f);
        rect2.origin = Panda::Point(0.6f, 0.6f);
        rect2.size = Panda::Size(0.2f, 0.2f);
        Panda::Renderer2D::drawRect(rect2);

        Panda::Renderer2D::RectData rect3;
        rect3.color = Panda::Color(0.f, 0.f, 1.f, 1.f);
        rect3.origin = Panda::Point(0.1f, 0.8f);
        rect3.size = Panda::Size(0.1f, 0.1f);
        Panda::Renderer2D::drawRect(rect3);

        if (Panda::Input::isMouseButtonPressed(Panda::MouseButton::LEFT)) {
            Panda::ParticleProps particleProps;
            particleProps.colorBegin = glm::vec4(0.0f, 0.0f, 0.9f, 1.0f);
            particleProps.colorEnd = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
            particleProps.sizeBegin = 0.1f;
            particleProps.sizeVariation = 0.1f;
            particleProps.sizeEnd = 0.0f;
            particleProps.lifeTime = 2.0f;
            particleProps.velocity = glm::vec2(0.0f, 0.0f);
            particleProps.velocityVariation = glm::vec2(0.7f, 0.7f);
            float x = Panda::Input::getMousePositionX();
            float y = Panda::Input::getMousePositionY();
            Panda::Point coord = m_camera->screenCoordToWorld(Panda::Point(x, y));
            particleProps.position = glm::vec2(coord.x, coord.y);
            for (int i = 0; i < 5; i++) {
                m_particleSystem->emit(particleProps);
            }
        }

        if (Panda::Input::isKeyJustPressed(Panda::Key::ESCAPE)) {
            Panda::Application::get()->close();
        }
    }

    void onImGuiRender() override {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        bool d = true;
        ImGui::Begin("DockSpace Frame", NULL, window_flags);
        ImGui::PopStyleVar(3);
        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                ImGui::MenuItem("Save", NULL);
                ImGui::MenuItem("Open", NULL);
                ImGui::Separator();

                if (ImGui::MenuItem("Exit", NULL)) {
                    Panda::Application::get()->close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Begin("Renderer2D statistics");
        ImGui::Text("FPS: %d", Panda::Application::get()->fps);
        auto stats = Panda::Renderer2D::getStats();
        ImGui::Text("Quads count: %d", stats.quadCount);
        ImGui::Text("Vertices count: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices count: %d", stats.getTotalIndexCount());
        ImGui::Text("Draw calls: %d", stats.drawCalls);
        ImGui::End();

        ImGui::Begin("Viewport");
        ImGui::Image((void *)(uintptr_t)colorAttachment,
            ImVec2(m_sceneSize.width, m_sceneSize.height),
            ImVec2(0, 1),
            ImVec2(1, 0));
        ImGui::End();

        // DOCKSPACE
        ImGui::End();
    }

    void setParticleSysyem(Foundation::Shared<Panda::ParticleSystem> particleSystem) {
        m_particleSystem = particleSystem;
    }

    void setCamera(Foundation::Shared<Panda::OrthographicCamera> camera) {
        m_camera = camera;
    }

private:
    Foundation::Shared<Panda::ParticleSystem> m_particleSystem;
    Foundation::Shared<Panda::OrthographicCamera> m_camera;
    Foundation::Shared<Panda::Texture> m_texture;
    Miren::FrameBufferHandle frameBuffer;
    Miren::TextureHandle colorAttachment;
    Miren::ViewId sceneViewId = 1;
    float degree = 0.f;
    float colorFactor = 0.f;
    Panda::Size m_sceneSize = Panda::Size(300, 200);
};

void ExampleLevel::start(Panda::World *world) {
    using namespace Miren;
    Foundation::Shared<Panda::OrthographicCamera> camera =
        Foundation::makeShared<Panda::OrthographicCamera>();
    Panda::Renderer2D::setCamera(camera);
    Foundation::Shared<Panda::Entity> entity = world->instantiateEntity();
    Foundation::Shared<Panda::ParticleSystem> particle =
        Foundation::makeShared<Panda::ParticleSystem>();
    Foundation::Shared<ExampleRenderer> dummy = Foundation::makeShared<ExampleRenderer>();
    Foundation::Shared<OrthographicCameraMove> cameraMove =
        Foundation::makeShared<OrthographicCameraMove>();
    entity->addComponent(dummy);
    entity->addComponent(particle);
    entity->addComponent(camera);
    entity->addComponent(cameraMove);
    cameraMove->setCamera(camera);
    dummy->setParticleSysyem(particle);
    dummy->setCamera(camera);
}
