#include <servers/rendering/rendering_device.hpp>

using namespace ism;

MEMBER_IMPL(RenderingDevice::singleton) {};

EMBED_CLASS(RenderingDevice, t, TypeFlags_BaseType | TypeFlags_IsAbstract) {}