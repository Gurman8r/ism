#include <servers/rendering_server.hpp>

using namespace ism;

VAR_IMPL(RenderingServer::singleton) {};

VAR_IMPL(RenderingServer::create_func) {};

OBJ_IMPL(RenderingServer, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}