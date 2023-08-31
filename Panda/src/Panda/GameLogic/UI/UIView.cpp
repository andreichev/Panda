#include "pndpch.hpp"
#include "Panda/Assets/AssetLoader.hpp"
#include "Panda/GameLogic/UI/UIView.hpp"
#include "Panda/Application/Application.hpp"

namespace Panda {

UIView::UIView()
    : UIView(Rect::zero()) {}

UIView::~UIView() {
    Miren::deleteVertexBuffer(vertexBufferHandle);
    Miren::deleteProgram(programHandle);
    Application::get()->removeWindowSizeObserver(this);
}

UIView::UIView(Rect frame)
    : window(Application::get()->getWindow())
    , frame(frame) {
    float *data = new float[12];
    ProgramAsset programAsset = AssetLoader::loadProgram(
        "shaders/ui/uiview_vertex.glsl", "shaders/ui/uiview_fragment.glsl");
    programHandle = Miren::createProgram(programAsset.getMirenProgramCreate());
    Miren::VertexBufferLayoutData layoutData;
    layoutData.pushFloat(2);
    Miren::VertexLayoutHandle vertexLayout = Miren::createVertexLayout(layoutData);
    vertexBufferHandle = Miren::createDynamicVertexBuffer(data, sizeof(float) * 12, vertexLayout);
    windowSize = window->getSize();
    // TODO: - Subscribe to window size events
    Application::get()->addWindowSizeObserver(this);
    layout();
}

void UIView::setFrame(Rect frame) {
    this->frame = frame;
    layout();
}

void UIView::addSubview(Foundation::Shared<UIView> node) {
    subviews.push_back(node);
    node->layout();
}

void UIView::removeSubview(Foundation::Shared<UIView> node) {
    subviews.erase(find(subviews.begin(), subviews.end(), node));
}

void UIView::render() {
    draw();
    for (auto node : subviews) {
        node->render();
    }
}

void UIView::layout() {
    float leftEdge = frame.origin.x / windowSize.width;
    float rightEdge = (frame.origin.x + frame.size.width) / windowSize.width;
    float topEdge = 1.f - frame.origin.y / windowSize.height;
    float bottomEdge = 1.f - (frame.origin.y + frame.size.height) / windowSize.height;
    float *data = new float[12]{rightEdge,
        topEdge,
        leftEdge,
        topEdge,
        leftEdge,
        bottomEdge,
        rightEdge,
        topEdge,
        leftEdge,
        bottomEdge,
        rightEdge,
        bottomEdge};
    for (int i = 0; i < 12; i++) {
        data[i] = data[i] * 2 - 1;
    }
    Miren::updateDynamicVertexBuffer(vertexBufferHandle, data, sizeof(float) * 12);
}

void UIView::draw() {
    Miren::setShader(programHandle);
    Miren::setVertexBuffer(vertexBufferHandle);
    // Miren::submitPrimitives(6);
}

void UIView::windowSizeChanged(Size size) {
    windowSize = size;
    layout();
}

} // namespace Panda