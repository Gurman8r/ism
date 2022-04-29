#include <servers/rendering_server.hpp>

using namespace ism;

MEMBER_IMPL(RenderingServer::singleton) {};

EMBED_OBJECT_CLASS(RenderingServer, t, TypeFlags_IsAbstract) {}

RenderingServer::~RenderingServer() {}