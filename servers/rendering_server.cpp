#include <servers/rendering_server.hpp>

using namespace ism;

MEMBER_IMPL(RenderingServer::singleton) {};

EMBEDED_CLASS(RenderingServer, t, TypeFlags_IsAbstract) {}