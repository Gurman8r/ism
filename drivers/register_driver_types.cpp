#include <drivers/register_driver_types.hpp>

#if OPENGL_ENABLED
#include <drivers/opengl/opengl_rendering_device.hpp>
#define RENDERING_DEVICE_DEFAULT OpenGlRenderingDevice
#endif

#if SYSTEM_WINDOWS
#include <drivers/windows/windows_file.hpp>
#endif

namespace Ism
{
	void register_core_driver_types()
	{
#if SYSTEM_WINDOWS
		REGISTER_CLASS(WindowsFile);
		WindowsFile::initialize();
#endif
	}

	void unregister_core_driver_types()
	{
#if SYSTEM_WINDOWS
		WindowsFile::finalize();
#endif
	}

	void register_driver_types()
	{
		REGISTER_CLASS(RENDERING_DEVICE_DEFAULT);
	}

	void unregister_driver_types()
	{
	}
}