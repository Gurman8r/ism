#include <servers/rendering_server.hpp>

using namespace ism;

MEMBER_IMPL(RenderingServer::g_singleton) {};

OBJECT_EMBED(RenderingServer, t, TypeFlags_IsAbstract) {}

RenderingServer::RenderingServer() { g_singleton = this; }

RenderingServer::~RenderingServer() {}