#include <core/os/main_loop.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MainLoop::~MainLoop() {}

	void MainLoop::initialize()
	{
	}

	bool MainLoop::iteration(float_t dt)
	{
		return false;
	}

	void MainLoop::finalize()
	{
	}

	void MainLoop::set_startup_script(OBJECT value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}