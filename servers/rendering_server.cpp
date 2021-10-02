#include <servers/rendering_server.hpp>

using namespace ism;

MEMBER_IMPL(RenderingServer::singleton) {};

MEMBER_IMPL(RenderingServer::create_func) {};

EMBEDDED_CLASS_TYPE(RenderingServer, t, TypeFlags_IsAbstract) {}