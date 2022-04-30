#include <servers/rendering_server.hpp>

using namespace ism;

MEMBER_IMPL(RenderingServer::singleton) {};

OBJECT_EMBED(RenderingServer, t, TypeFlags_IsAbstract) {}

RenderingServer::~RenderingServer() {}