#include <servers/rendering_server.hpp>

using namespace ism;

MEMBER_IMPL(RenderingServer::singleton) {};

MEMBER_IMPL(RenderingServer::create_func) {};

OBJECT_IMPL(RenderingServer, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}