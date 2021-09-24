#include <servers/rendering/rendering_device.hpp>

using namespace ism;

MEMBER_IMP(RenderingDevice::singleton) {};

OBJECT_IMP(RenderingDevice, t, TypeFlags_BaseType | TypeFlags_IsAbstract)
{
}

RenderingDevice::~RenderingDevice()
{
}