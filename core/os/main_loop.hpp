#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/io/event.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop : public Object
	{
		OBJECT_COMMON(MainLoop, Object);

		OBJ m_script{};

	protected:
		MainLoop() noexcept {}

	public:
		virtual ~MainLoop() noexcept override = default;

		virtual void initialize();

		virtual bool process(Duration const & dt);

		virtual void finalize();

		void set_startup_script(OBJ const & value) { m_script = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_LOOP_HPP_
