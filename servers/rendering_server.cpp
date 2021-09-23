#include <servers/rendering_server.hpp>

using namespace ism;

MEMBER_IMPL(RenderingServer::singleton) {};

OBJ_IMPL(RenderingServer, t, "rendering_server", TypeFlags_BaseType | TypeFlags_IsAbstract)
{
}