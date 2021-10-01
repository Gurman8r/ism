#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/api/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop : public Object
	{
		OBJECT_COMMON(MainLoop, Object);

		OBJ m_script{};

	protected:
		MainLoop() noexcept : Object{} {}

	public:
		virtual ~MainLoop();

		virtual void initialize();

		virtual bool process(Duration const & delta_time);

		virtual void finalize();

		void set_startup_script(OBJ const & value) { m_script = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_LOOP_HPP_
