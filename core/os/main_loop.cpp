#include <core/os/main_loop.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(MainLoop, t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MainLoop::~MainLoop()
{
}

void MainLoop::initialize()
{
	STR_IDENTIFIER(_initialize);
	if (FUNCTION callback{ getattr(m_script, &ID__initialize) })
	{
		call_object(callback);
	}
}

bool MainLoop::process(Duration const & delta_time)
{
	bool should_close{};

	STR_IDENTIFIER(_process);
	if (FUNCTION callback{ getattr(m_script, &ID__process) })
	{
		static FloatObject dt; dt = delta_time.count();

		static ListObject args{ FLT(&dt), };
		OBJ result{ call_object(callback, &args) };
		if (result && result.cast<bool>()) {
			should_close = true;
		}
	}

	return should_close;
}

void MainLoop::finalize()
{
	STR_IDENTIFIER(_finalize);
	if (FUNCTION callback{ getattr(m_script, &ID__finalize) })
	{
		call_object(callback);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */