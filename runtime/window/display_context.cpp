#include <runtime/window/display_context.hpp>

using namespace ism;

MEMBER_IMPL(DisplayContext::singleton) {};

OBJECT_IMPL(DisplayContext, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}