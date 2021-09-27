#include <servers/display_server.hpp>

using namespace ism;

MEMBER_IMPL(DisplayServer::singleton) {};

OBJECT_IMPL(DisplayServer, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}