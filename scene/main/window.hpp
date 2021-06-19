#ifndef _ISM_WINDOW_HPP_
#define _ISM_WINDOW_HPP_

#include <scene/main/viewport.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Window : public Viewport
	{
		ISM_SUPER(Window, Viewport);
		
	public:
		virtual ~Window();

		explicit Window(Node * parent, SceneTree * tree) : Viewport{ parent, tree } {}

		explicit Window(SceneTree * tree) : Viewport{ tree } {}

		explicit Window(Node * parent) : Viewport{ parent } {}

		NON_COPYABLE(Window);

		MOVABLE(Window);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_WINDOW_HPP_
