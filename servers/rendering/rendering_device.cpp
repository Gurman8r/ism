#include <servers/rendering/rendering_device.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(RenderingDevice, t, "rendering_device", TypeFlags_BaseType | TypeFlags_IsAbstract)
{
	t.tp_base = typeof<Object>();
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