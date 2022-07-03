#include <servers/display_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(WindowCharEvent, t) {}

OBJECT_EMBED(WindowCharModsEvent, t) {}

OBJECT_EMBED(WindowCloseEvent, t) {}

OBJECT_EMBED(WindowContentScaleEvent, t) {}

OBJECT_EMBED(WindowDropEvent, t) {}

OBJECT_EMBED(WindowFocusEvent, t) {}

OBJECT_EMBED(WindowFramebufferResizeEvent, t) {}

OBJECT_EMBED(WindowIconifyEvent, t) {}

OBJECT_EMBED(WindowKeyEvent, t) {}

OBJECT_EMBED(WindowMaximizeEvent, t) {}

OBJECT_EMBED(WindowMouseButtonEvent, t) {}

OBJECT_EMBED(WindowMouseEnterEvent, t) {}

OBJECT_EMBED(WindowMousePositionEvent, t) {}

OBJECT_EMBED(WindowScrollEvent, t) {}

OBJECT_EMBED(WindowPositionEvent, t) {}

OBJECT_EMBED(WindowRefreshEvent, t) {}

OBJECT_EMBED(WindowSizeEvent, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(DisplayServer::g_singleton) {};

OBJECT_EMBED(DisplayServer, t, TypeFlags_IsAbstract) {}

DisplayServer::DisplayServer() { g_singleton = this; }

DisplayServer::~DisplayServer() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */