#include <servers/rendering/rendering_device.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(RenderingDevice::g_singleton) {};

OBJECT_EMBED(RenderingDevice, t, TypeFlags_IsAbstract) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */