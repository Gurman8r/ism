#include <runtime/renderer/rendering_context.hpp>

using namespace ism;

MEMBER_IMPL(RenderingContext::singleton) {};

MEMBER_IMPL(RenderingContext::create_func) {};

OBJECT_IMPL(RenderingContext, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}