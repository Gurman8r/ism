#include <core/os/main_loop.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(MainLoop, t)
	{
		t.tp_bind = CLASS_(MainLoop, klass)
		{
			return klass
				.def("initialize", &MainLoop::initialize)
				.def("process", &MainLoop::process)
				.def("finalize", &MainLoop::finalize)
				.def("set_initialize_script", &MainLoop::set_initialize_script)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void MainLoop::initialize()
	{
		_initialize();
	}

	bool MainLoop::physics_process(Duration const & dt)
	{
		return _physics_process(dt);
	}

	bool MainLoop::process(Duration const & dt)
	{
		return _process(dt);
	}

	void MainLoop::finalize()
	{
		_finalize();
	}

	void MainLoop::set_initialize_script(Ref<Script> const & value)
	{
		m_script = value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}