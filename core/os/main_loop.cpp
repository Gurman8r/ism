#include <core/os/main_loop.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MainLoop::~MainLoop() {}

	void MainLoop::initialize()
	{
	}

	bool MainLoop::process(Duration delta_time)
	{
		return true;
	}

	void MainLoop::finalize()
	{
	}

	void MainLoop::set_startup_script(OBJ value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}