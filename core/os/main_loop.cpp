#include <core/os/main_loop.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(MainLoop, t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MainLoop::~MainLoop()
{
}

void MainLoop::initialize()
{
}

bool MainLoop::process(Duration const & delta_time)
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