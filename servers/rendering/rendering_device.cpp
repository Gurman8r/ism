#include <servers/rendering/rendering_device.hpp>

using namespace ism;

MEMBER_IMPL(RenderingDevice::singleton) {};

OBJ_IMPL(RenderingDevice, t, "rendering_device", TypeFlags_BaseType | TypeFlags_IsAbstract)
{
}

RenderingDevice::~RenderingDevice()
{
}