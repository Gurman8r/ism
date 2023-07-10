#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/object/script.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum : Notification_ {
		Notification_OS_MemoryWarning = 2001,
		Notification_TranslationChanged,
		Notification_OS_IME_Update,
		Notification_WM_About,
		Notification_Crash,
		Notification_ApplicationResumed,
		Notification_ApplicationPaused,
		Notification_ApplicationFocusIn,
		Notification_ApplicationFocusOut,
		Notification_TextServerChanged,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop : public Object
	{
		OBJECT_CLASS(MainLoop, Object);

		Ref<Script> m_script{};

		virtual void _initialize() {}
		virtual bool _physics_process(Duration const & dt) { return false; }
		virtual bool _process(Duration const & dt) { return false; }
		virtual void _finalize() {}

	public:
		MainLoop() noexcept {}
		virtual ~MainLoop() noexcept override {}

		virtual void initialize();
		virtual bool physics_process(Duration const & dt);
		virtual bool process(Duration const & dt);
		virtual void finalize();

		void set_initialize_script(Ref<Script> const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_LOOP_HPP_
