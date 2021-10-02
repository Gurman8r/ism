#include <servers/rendering/rendering_device.hpp>

using namespace ism;

MEMBER_IMPL(RenderingDevice::singleton) {};

EMBEDDED_CLASS_TYPE(RenderingDevice, t, TypeFlags_IsAbstract) {}

RenderingDevice::RenderingDevice() : Object{}
{
	singleton = this;
}

RenderingDevice::~RenderingDevice()
{
}