#include <servers/display_server.hpp>

using namespace ism;

VAR_IMPL(DisplayServer::singleton) {};

OBJ_IMPL(DisplayServer, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}