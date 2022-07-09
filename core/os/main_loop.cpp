#include <core/os/main_loop.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(MainLoop, t)
	{
		t.tp_install = CLASS_INSTALLER(MainLoop, t)
		{
			return t
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
		STR_IDENTIFIER(initialize);

		if (m_script)
		{
			if (OBJ callback{ getattr(m_script, &ID_initialize) })
			{
				call_object(callback);
			}
		}
	}

	bool MainLoop::process(Duration const & dt)
	{
		STR_IDENTIFIER(process);

		bool should_close{};

		if (m_script)
		{
			if (OBJ callback{ getattr(m_script, &ID_process) })
			{
				static FloatObject arg0; arg0 = dt.count();
				static ListObject args{ &arg0, };
				OBJ result{ call_object(callback, &args) };
				if (result && result.cast<bool>())
				{
					should_close = true;
				}
			}
		}

		return should_close;
	}

	void MainLoop::finalize()
	{
		STR_IDENTIFIER(finalize);

		if (m_script)
		{
			if (OBJ callback{ getattr(m_script, &ID_finalize) })
			{
				call_object(callback);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}