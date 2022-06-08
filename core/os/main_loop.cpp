#include <core/os/main_loop.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(MainLoop, t)
{
	t.tp_install = CLASS_INSTALLER(MainLoop, t)
	{
		return t
			.def("initialize", &MainLoop::initialize)
			.def("process", &MainLoop::process)
			.def("finalize", &MainLoop::finalize)
			.def("handle_event", &MainLoop::handle_event)
			.def("set_startup_script", &MainLoop::set_startup_script)
			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MainLoop::initialize()
{
	if (STR_IDENTIFIER(initialize); m_script) {
		if (OBJ callback{ getattr(m_script, &ID_initialize) }) {
			call_object(callback);
		}
	}
}

bool MainLoop::process(Duration const & dt)
{
	bool should_close{};

	if (STR_IDENTIFIER(process); m_script) {
		if (OBJ callback{ getattr(m_script, &ID_process) }) {
			static FloatObject arg0; arg0 = dt.count();
			static ListObject args{ &arg0, };
			OBJ result{ call_object(callback, &args) };
			if (result && result.cast<bool>()) {
				should_close = true;
			}
		}
	}

	return should_close;
}

void MainLoop::finalize()
{
	if (STR_IDENTIFIER(finalize); m_script) {
		if (OBJ callback{ getattr(m_script, &ID_finalize) }) {
			call_object(callback);
		}
	}
}

void MainLoop::handle_event(Event const & event)
{
	if (STR_IDENTIFIER(handle_event); m_script) {
		if (OBJ callback{ getattr(m_script, &ID_handle_event) }) {
			callback((Event &)event);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */