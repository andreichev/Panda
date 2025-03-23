#pragma once

#include "Fern/Window/Window.hpp"
#include "Fern/Events/EventQueue.hpp"

namespace Fern {

void initialize();
void terminate();
Window *
createWindow(const char *title, Rect rect, WindowState state, DrawingContextType contextType);
Vec2 getCursorPosition();
const char *getClipboardText();
void setClipboardText(const char *text);
void toggleCursorLock();
bool isCursorLocked();
void setCursor(Cursor cursor);
void pollEvents();
EventQueue *getEventQueue();

} // namespace Fern
