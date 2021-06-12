#ifndef _ISM_WINDOW_HPP_
#define _ISM_WINDOW_HPP_

#include <scene/main/viewport.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Window : public Viewport
	{
		ISM_SUPER_CLASS(Window, Viewport);

	private:

	public:
		virtual ~Window() override = default;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(Window);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOW_HPP_
