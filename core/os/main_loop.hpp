#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/api/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop : public Object
	{
		ISM_OBJECT_DEFAULT(MainLoop, Object);

		OBJ m_startup_script{};

	protected:
		MainLoop() noexcept : Object{ get_class() } {}

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
