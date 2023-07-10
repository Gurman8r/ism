#include <servers/rendering/rendering_device.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_EMBED(RenderingDevice);

	OBJECT_EMBED(RenderingDevice, t, TypeFlags_IsAbstract) {}

	RenderingDevice::RenderingDevice() noexcept { SINGLETON_CTOR(); }

	RenderingDevice::~RenderingDevice() noexcept { SINGLETON_DTOR(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}