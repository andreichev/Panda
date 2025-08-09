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
    , m_frame(frame)
    , m_transform(1.f) {
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

void View::notifyWillCalculateLayout() {
    viewWillCalculateLayout();
    for (auto view : m_subviews) {
        view->notifyWillCalculateLayout();
    }
}

void View::styleSetSize(Size size) {
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeStyleSetWidth(thisYogaView, size.width);
    YGNodeStyleSetHeight(thisYogaView, size.height);
}

void View::styleSetSizePercent(Size size) {
    PND_ASSERT(
        size.width >= 0 && size.width <= 100 && size.height >= 0 && size.height <= 100,
        "Percent values should be between 0 and 100"
    );
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeStyleSetWidthPercent(thisYogaView, size.width);
    YGNodeStyleSetHeightPercent(thisYogaView, size.height);
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

void View::styleSetOriginPercent(Point point) {
    PND_ASSERT(
        point.x >= 0 && point.x <= 100 && point.y >= 0 && point.y <= 100,
        "Percent values should be between 0 and 100"
    );
    auto *internal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = internal->ref;
    YGNodeStyleSetPositionPercent(thisYogaView, YGEdgeLeft, point.x);
    YGNodeStyleSetPositionPercent(thisYogaView, YGEdgeTop, point.y);
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

void View::addSubview(Foundation::Shared<View> view) {
    PND_ASSERT(view != nullptr, "INVALID NODE");
    m_subviews.push_back(view);
    auto *thisInternal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = thisInternal->ref;
    auto thatInternal = (ViewInternal *)view->m_internal;
    YGNodeRef thatYogaView = thatInternal->ref;
    int subviewsCount = YGNodeGetChildCount(thisYogaView);
    YGNodeInsertChild(thisYogaView, thatYogaView, subviewsCount);
}

void View::resolveHierarchy() {
    auto *thisInternal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = thisInternal->ref;
    YGNodeRemoveAllChildren(thisYogaView);
    for (auto view : m_subviews) {
        auto *thatInternal = (ViewInternal *)view->m_internal;
        YGNodeRef thatYogaView = thatInternal->ref;
        int subviewsCount = YGNodeGetChildCount(thisYogaView);
        YGNodeInsertChild(thisYogaView, thatYogaView, subviewsCount);
        view->resolveHierarchy();
    }
}

void View::removeSubview(Foundation::Shared<View> node) {
    PND_ASSERT(node != nullptr, "INVALID NODE");
    auto *thisInternal = (ViewInternal *)m_internal;
    YGNodeRef thisYogaView = thisInternal->ref;
    auto *thatInternal = (ViewInternal *)m_internal;
    YGNodeRef thatYogaView = thatInternal->ref;
    YGNodeRemoveChild(thisYogaView, thatYogaView);
    PND_DISCARD(std::remove(m_subviews.begin(), m_subviews.end(), node));
}

void View::calculateLayout() {
    notifyWillCalculateLayout();
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
    for (auto subview : m_subviews) {
        subview->applyCalculatedLayout();
    }
}

void View::render(float offsetX, float offsetY) {
    Point origin = m_frame.origin;
    for (auto subview : m_subviews) {
        subview->render(origin.x + offsetX, origin.y + offsetY);
    }
    UIRenderer::RectData rect;
    origin.x += offsetX;
    origin.y += offsetY;
    Size size = m_frame.size;
    rect.center = Panda::Vec3(origin.x + size.width / 2, origin.y + size.height / 2, 0.f);
    rect.size = Panda::Size(size.width, size.height);
    Color color = m_backgroundColor;
    rect.color = {color.r, color.g, color.b, color.a};
    rect.transform = m_transform;
    Context::shared().getRenderer().drawRect(rect);
}

} // namespace PandaUI
