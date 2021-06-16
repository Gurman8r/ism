#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/api/object/generic_object.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop// : public Super
	{
		//ISM_SUPER(MainLoop, Super);

	private:
		api::OBJECT m_startup_script{};

	public:
		virtual ~MainLoop();

		virtual void initialize();

		virtual bool iteration(float_t dt);

		virtual void finalize();

		void set_startup_script(api::OBJECT value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_LOOP_HPP_
