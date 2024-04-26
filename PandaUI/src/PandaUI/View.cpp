#include "PandaUI/View.hpp"
#include "PandaUI/Context.hpp"

#include <Panda/Renderer/Renderer2D.hpp>
#include <yoga/Yoga.h>

namespace PandaUI {

struct ViewInternal {
    YGNodeRef ref;
};

View::View()
    : View(Rect::zero()) {}

View::View(Rect frame)
    : m_backgroundColor(1.f, 1.f, 1.f, 1.f)
    , m_frame(frame) {
    YGNodeRef thisYogaView = YGNodeNew();
    auto *internal = (ViewInternal *)m_internal;
    internal->ref = thisYogaView;
    LOG_INFO("YOGA VIEW CREATED");
}

View::~View() {
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeFree(thisYogaView);
    LOG_INFO("YOGA VIEW FREED");
}

void View::styleSetSize(Size size) {
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeStyleSetWidth(thisYogaView, size.width);
    YGNodeStyleSetHeight(thisYogaView, size.height);
}

void View::styleSetMargins(EdgeInsets margins) {
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeStyleSetMargin(thisYogaView, YGEdgeTop, margins.top);
    YGNodeStyleSetMargin(thisYogaView, YGEdgeRight, margins.right);
    YGNodeStyleSetMargin(thisYogaView, YGEdgeBottom, margins.bottom);
    YGNodeStyleSetMargin(thisYogaView, YGEdgeLeft, margins.left);
}

void View::styleSetOrigin(Point point) {
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeStyleSetPosition(thisYogaView, YGEdgeLeft, point.x);
    YGNodeStyleSetPosition(thisYogaView, YGEdgeTop, point.y);
}

void View::styleReset() {
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeReset(thisYogaView);
}

void View::styleSetRelative() {
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeStyleSetPositionType(thisYogaView, YGPositionTypeRelative);
}

void View::styleSetAbsolute() {
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeStyleSetPositionType(thisYogaView, YGPositionTypeAbsolute);
}

void View::styleSetInsetsFromParent(EdgeInsets insets) {
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeStyleSetPosition(thisYogaView, YGEdgeTop, insets.top);
    YGNodeStyleSetPosition(thisYogaView, YGEdgeRight, insets.right);
    YGNodeStyleSetPosition(thisYogaView, YGEdgeBottom, insets.bottom);
    YGNodeStyleSetPosition(thisYogaView, YGEdgeLeft, insets.left);
}

void View::addSubview(Foundation::Shared<View> node) {
    PND_ASSERT(node != nullptr, "INVALID NODE");
    auto *thisInternal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = thisInternal->ref;
    auto *thatInternal = (ViewInternal *)node->m_internal;
    YGNodeRef thatYogaView = thatInternal->ref;
    int subviewsCount = YGNodeGetChildCount(thatYogaView);
    YGNodeInsertChild(thisYogaView, thatYogaView, subviewsCount);
    m_subviews.push_back(node);
}

void View::removeSubview(Foundation::Shared<View> node) {
    PND_ASSERT(node != nullptr, "INVALID NODE");
    auto *thisInternal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = thisInternal->ref;
    auto *thatInternal = (ViewInternal *)m_internal;
    YGNodeRef thatYogaView = thatInternal->ref;
    YGNodeRemoveChild(thisYogaView, thatYogaView);
    std::remove(m_subviews.begin(), m_subviews.end(), node);
}

void View::calculateLayout() {
    auto *thisInternal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = thisInternal->ref;
    YGNodeCalculateLayout(thisYogaView, YGUndefined, YGUndefined, YGDirectionLTR);
    applyCalculatedLayout();
}

void View::applyCalculatedLayout() {
    auto *thisInternal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = thisInternal->ref;
    m_frame.origin.x = YGNodeLayoutGetLeft(thisYogaView);
    m_frame.origin.y = YGNodeLayoutGetTop(thisYogaView);
    m_frame.size.width = YGNodeLayoutGetWidth(thisYogaView);
    m_frame.size.height = YGNodeLayoutGetHeight(thisYogaView);
    for (auto &subview : m_subviews) {
        subview->applyCalculatedLayout();
    }
}

void View::render(float offsetX, float offsetY) {
    Panda::Renderer2D::RectData rect;
    Point origin = getFrame().origin;
    origin.x += offsetX;
    origin.y += offsetY;
    Size size = getFrame().size;
    rect.center = Panda::Vec3(origin.x + size.width / 2, origin.y + size.height / 2, 0.f);
    rect.size = Panda::Size(size.width, size.height);
    Color color = getBackgroundColor();
    rect.color = {color.r, color.g, color.b, color.a};
    Context::shared().getRenderer().drawRect(rect);
    for (auto view : m_subviews) {
        view->render(origin.x + offsetX, origin.y + offsetY);
    }
}

} // namespace PandaUI