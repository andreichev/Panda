#include "PandaUI/View.hpp"
#include "PandaUI/Context.hpp"

#include <Panda/Renderer/Renderer2D.hpp>

namespace PandaUI {

View::View()
    : View(Rect::zero()) {}

View::View(Rect frame)
    : m_frame(frame)
    , m_backgroundColor(1.f, 1.f, 1.f, 1.f) {}

View::~View() {}

void View::setFrame(Rect frame) {
    m_frame = frame;
    layout();
}

void View::addSubview(View *node) {
    m_subviews.push_back(node);
    node->layout();
}

void View::removeSubview(View *node) {
    m_subviews.erase(find(m_subviews.begin(), m_subviews.end(), node));
}

void View::layout() {
    for (View *view : m_subviews) {
        view->layout();
    }
}

void View::render() {
    Panda::Renderer2D::RectData rect;
    Point origin = getFrame().origin;
    Size size = getFrame().size;
    rect.center = Panda::Vec3(origin.x + size.width / 2, origin.y + size.height / 2, 0.f);
    rect.size = Panda::Size(size.width, size.height);
    Color color = getBackgroundColor();
    rect.color = {color.r, color.g, color.b, color.a};
    Context::shared().getRenderer().drawRect(rect);
    for (View *view : m_subviews) {
        view->render();
    }
}

} // namespace PandaUI