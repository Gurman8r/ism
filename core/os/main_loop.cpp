#include <core/os/main_loop.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDED_CLASS(MainLoop, t)
{
	CLASS_DEF(MainLoop, t)
	{
		return t

			.def("_initialize", &MainLoop::initialize)
			
			.def("_process", &MainLoop::process)
			
			.def("_finalize", &MainLoop::finalize)

			.def("set_startup_script", &MainLoop::set_startup_script)

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MainLoop::initialize()
{
	// _initialize
	STR_IDENTIFIER(_initialize);
	if (OBJ callback{ getattr(m_script, &ID__initialize) }) {
		call_object(callback);
	}
}

bool MainLoop::process(Duration const & dt)
{
	bool should_close{};

	// _process
	STR_IDENTIFIER(_process);
	if (OBJ callback{ getattr(m_script, &ID__process) }) {
		static FloatObject arg0; arg0 = dt.count();
		static ListObject args{ &arg0, };
		OBJ result{ call_object(callback, &args) };
		if (result && result.cast<bool>()) {
			should_close = true;
		}
	}

	return should_close;
}

void MainLoop::finalize()
{
	// _finalize
	STR_IDENTIFIER(_finalize);
	if (OBJ callback{ getattr(m_script, &ID__finalize) }) {
		call_object(callback);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */