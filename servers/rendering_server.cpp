#include <servers/rendering_server.hpp>

using namespace ism;

DECLEXPR(ism::RenderingServer::singleton) {};

ISM_OBJECT_IMPL(RenderingServer, t, "rendering_server", TypeFlags_BaseType | TypeFlags_IsAbstract)
{
}