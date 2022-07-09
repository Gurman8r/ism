#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop : public Object
	{
		OBJECT_COMMON(MainLoop, Object);

		OBJ m_script{};

	public:
		enum
		{
			Notification_MemoryWarning = 2001,
			Notification_Crash,
			Notification_ApplicationResumed,
			Notification_ApplicationPaused,
			Notification_ApplicationFocusIn,
			Notification_ApplicationFocusOut,
		};

		MainLoop() noexcept {}
		virtual ~MainLoop() noexcept override {}

		virtual void initialize();
		virtual bool process(Duration const & dt);
		virtual void finalize();

		void set_startup_script(OBJ const & value) { m_script = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_LOOP_HPP_
