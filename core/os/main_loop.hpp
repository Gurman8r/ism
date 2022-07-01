#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/io/event.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop : public EventHandler
	{
		OBJECT_COMMON(MainLoop, EventHandler);

		OBJ m_script{};

	protected:
		explicit MainLoop(EventBus * bus = nullptr) noexcept : EventHandler{ bus } {}

	public:
		virtual ~MainLoop() noexcept override = default;

		virtual void initialize();

		virtual bool process(Duration const & dt);

		virtual void finalize();

		virtual void handle_event(Event const & value) override;

		void set_startup_script(OBJ const & value) { m_script = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_LOOP_HPP_
