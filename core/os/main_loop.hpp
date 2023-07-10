#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum : Notification_ {
		Notification_MemoryWarning = 2001,
		Notification_Crash,
		Notification_ApplicationResumed,
		Notification_ApplicationPaused,
		Notification_ApplicationFocusIn,
		Notification_ApplicationFocusOut,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop : public Object
	{
		OBJECT_CLASS(MainLoop, Object);

		ObjectRef m_script{};

	public:
		

		MainLoop() noexcept {}
		virtual ~MainLoop() noexcept override {}

		virtual void initialize();
		virtual bool physics_process(Duration const & dt);
		virtual bool process(Duration const & dt);
		virtual void finalize();

		void set_startup_script(ObjectRef const & value) noexcept {
			if (m_script != value) {
				m_script = value;
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_LOOP_HPP_
