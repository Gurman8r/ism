#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop : public Object
	{
		OBJ_CLASS(MainLoop, Object);

		OBJ m_startup_script{};

	protected:
		MainLoop() noexcept : Object{} {}

	public:
		virtual ~MainLoop();

		virtual void initialize();

		virtual bool process(Duration delta_time);

		virtual void finalize();

		void set_startup_script(OBJ value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_LOOP_HPP_
