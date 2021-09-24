#include <servers/rendering_server.hpp>

using namespace ism;

MEMBER_IMP(RenderingServer::singleton) {};

MEMBER_IMP(RenderingServer::create_func) {};

OBJECT_IMP(RenderingServer, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}