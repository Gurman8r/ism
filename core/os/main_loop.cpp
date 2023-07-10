#include <core/os/main_loop.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(MainLoop, t)
	{
		t.tp_bind = BIND_CLASS(MainLoop, klass)
		{
			return klass
				.def("initialize", &MainLoop::initialize)
				.def("process", &MainLoop::process)
				.def("finalize", &MainLoop::finalize)
				.def("set_startup_script", &MainLoop::set_startup_script)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void MainLoop::initialize()
	{
		STRING_IDENTIFIER(initialize);

		if (ObjectRef fn; m_script && (fn = getattr(m_script, &ID_initialize)))
		{
			call_object(fn);
		}
	}

	bool MainLoop::physics_process(Duration const & dt)
	{
		STRING_IDENTIFIER(physics_process);

		bool should_close{};

		if (ObjectRef fn; m_script && (fn = getattr(m_script, &ID_physics_process)))
		{
			static FloatObject arg0; arg0 = dt.count();
			static ListObject args{ &arg0, };
			ObjectRef result{ call_object(fn, &args) };
			if (result && result.cast<bool>()) {
				should_close = true;
			}
		}

		return should_close;
	}

	bool MainLoop::process(Duration const & dt)
	{
		STRING_IDENTIFIER(process);

		bool should_close{};

		if (ObjectRef fn; m_script && (fn = getattr(m_script, &ID_process)))
		{
			static FloatObject arg0; arg0 = dt.count();
			static ListObject args{ &arg0, };
			ObjectRef result{ call_object(fn, &args) };
			if (result && result.cast<bool>()) {
				should_close = true;
			}
		}

		return should_close;
	}

	void MainLoop::finalize()
	{
		STRING_IDENTIFIER(finalize);

		if (ObjectRef fn; m_script && (fn = getattr(m_script, &ID_finalize)))
		{
			call_object(fn);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}