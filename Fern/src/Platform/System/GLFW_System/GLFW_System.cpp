#include "GLFW_System.hpp"
#include "GlfwWindow/GLFW_Window.hpp"

// We gather version tests as define in order to easily see which features are version-dependent.
#define GLFW_VERSION_COMBINED                                                                      \
    (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 + GLFW_VERSION_REVISION)
#ifdef GLFW_RESIZE_NESW_CURSOR // Let's be nice to people who pulled GLFW between 2019-04-16 (3.4
                               // define) and 2019-11-29 (cursors defines) // FIXME: Remove when
                               // GLFW 3.4 is released?
#    define GLFW_HAS_NEW_CURSORS                                                                   \
        (GLFW_VERSION_COMBINED >= 3400) // 3.4+ GLFW_RESIZE_ALL_CURSOR, GLFW_RESIZE_NESW_CURSOR,
                                        // GLFW_RESIZE_NWSE_CURSOR, GLFW_NOT_ALLOWED_CURSOR
#else
#    define GLFW_HAS_NEW_CURSORS (0)
#endif

namespace Fern {

void glfwErrorCallback(int error_code, const char *description) {
    LOG_ERROR("GLFW ERROR, code: %d, description: %s", error_code, description);
}

GLFW_System::GLFW_System()
    : m_isCursorLocked(false) {
    LOG_INFO("Hello GLFW! %s", glfwGetVersionString());
    glfwSetErrorCallback(glfwErrorCallback);
    if (glfwInit() == false) {
        LOG_CRITICAL("GLFW INITIALIZATION ERROR");
        exit(1);
    }
    m_cursors[Cursor::ARROW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_cursors[Cursor::IBEAM] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    m_cursors[Cursor::CROSSHAIR] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    m_cursors[Cursor::POINTING_HAND] = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
    m_cursors[Cursor::RESIZE_EW] = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    m_cursors[Cursor::RESIZE_NS] = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
#if GLFW_HAS_NEW_CURSORS
    m_cursors[Cursor::RESIZE_NESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
    m_cursors[Cursor::RESIZE_NWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
    m_cursors[Cursor::RESIZE_ALL] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
    m_cursors[Cursor::NOT_ALLOWED] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
#else
    m_cursors[Cursor::RESIZE_NESW] = m_cursors[Cursor::ARROW];
    m_cursors[Cursor::RESIZE_NWSE] = m_cursors[Cursor::ARROW];
    m_cursors[Cursor::RESIZE_ALL] = m_cursors[Cursor::ARROW];
    m_cursors[Cursor::NOT_ALLOWED] = m_cursors[Cursor::ARROW];
#endif
}

Window *GLFW_System::createWindow(
    const char *title, Rect rect, WindowState state, DrawingContextType contextType
) {
    GLFW_Window *window =
        F_NEW(Foundation::getAllocator(), GLFW_Window)(title, rect, state, contextType);
    m_windows.push_back(window);
    return window;
}

void GLFW_System::disposeWindow(Window *window) {
    if (window->getState() == WindowState::WindowStateFullScreen) {
        window->setState(WindowState::WindowStateNormal);
    }
    F_DELETE(Foundation::getAllocator(), window);
    m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), window), m_windows.end());
}

void GLFW_System::pollEvents() {
    glfwPollEvents();
}

Vec2 GLFW_System::getCursorPosition() const {
    GLFW_Window *window = getFocusedWindow();
    if (!window) { return Vec2(); }
    double cposx, cposy;
    glfwGetCursorPos(window->getHandle(), &cposx, &cposy);
    int wposx, wposy;
    glfwGetWindowPos(window->getHandle(), &wposx, &wposy);
    float globalPosx = cposx + wposx;
    float globalPosy = cposy + wposy;
    return {globalPosx, globalPosy};
}

const char *GLFW_System::getClipboardText() {
    GLFW_Window *window = getFocusedWindow();
    if (!window) { return nullptr; }
    return glfwGetClipboardString(window->getHandle());
}

void GLFW_System::setClipboardText(const char *text) {
    GLFW_Window *window = getFocusedWindow();
    if (!window) { return; }
    glfwSetClipboardString(window->getHandle(), text);
}

void GLFW_System::toggleCursorLock() {
    GLFW_Window *window = getFocusedWindow();
    if (!window) { return; }
    m_isCursorLocked = !m_isCursorLocked;
    resetCursorPos();
    glfwSetInputMode(
        window->getHandle(),
        GLFW_CURSOR,
        m_isCursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
    );
}

bool GLFW_System::isCursorLocked() {
    return m_isCursorLocked;
}

void GLFW_System::setCursor(Cursor cursor) {
    GLFW_Window *window = getFocusedWindow();
    if (!window) { return; }
    GLFWcursor *glfwCursor = m_cursors[cursor];
    glfwSetCursor(window->getHandle(), glfwCursor);
}

uint64_t GLFW_System::getMilliSeconds() const {
    return glfwGetTime() * 1000;
}

GLFW_Window *GLFW_System::getFocusedWindow() const {
    if (m_windows.empty()) { return nullptr; }
    for (auto window : m_windows) {
        if (window->isFocused()) { return window; }
    }
    return m_windows[0];
}

void GLFW_System::resetCursorPos() {
    GLFW_Window *window = getFocusedWindow();
    if (!window) { return; }
    Size size = window->getSize();
    int x = size.width / 2;
    int y = size.height / 2;
    glfwSetCursorPos(window->getHandle(), x, y);
    window->ignoreNextMouseEvents();
    getEventQueue()->postMouseEvent(x, y, 0, 0);
}

} // namespace Fern