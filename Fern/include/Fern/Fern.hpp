#pragma once

#include <Foundation/IO/StreamReaderI.hpp>
#include <filesystem>

#include "Fern/Window/Window.hpp"
#include "Fern/Events/EventQueue.hpp"

namespace Fern {

void initialize();
void terminate();
/// Never explicitly delete the window, use disposeWindow() instead.
Window *
createWindow(const char *title, Rect rect, WindowState state, DrawingContextType contextType);
void disposeWindow(Window *window);
Vec2 getCursorPosition();
const char *getClipboardText();
void setClipboardText(const char *text);
void toggleCursorLock();
bool isCursorLocked();
void setCursor(Cursor cursor);
void pollEvents();
EventQueue *getEventQueue();
const std::filesystem::path &getStaticResourcesPath();
Foundation::StreamReaderI *createStaticResourceReader(const std::filesystem::path &path);
void disposeResourceReader(Foundation::StreamReaderI *reader);

} // namespace Fern
