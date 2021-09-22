#include <servers/rendering/rendering_device.hpp>

using namespace ism;

ISM_OBJECT_IMPL(RenderingDevice, t, "rendering_device", TypeFlags_BaseType | TypeFlags_IsAbstract)
{
}

RenderingDevice::~RenderingDevice()
{
}

RenderingDevice * ism::get_render_device()
{
	return nullptr;
}

RenderingDevice * ism::set_render_device(RenderingDevice * value)
{
	return nullptr;
}