#include <servers/rendering/rendering_device.hpp>

using namespace ism;

MEMBER_IMPL(RenderingDevice::singleton) {};

EMBED_OBJECT_CLASS(RenderingDevice, t, TypeFlags_IsAbstract) {}