#ifndef _ISM_BUTTON_HPP_
#define _ISM_BUTTON_HPP_

#include <scene/gui/gui_node.hpp>

namespace ism
{
	class ISM_API Button : public GuiNode
	{
		OBJECT_COMMON(Button, GuiNode);

	public:
		virtual ~Button();
	};
}

#endif // !_ISM_BUTTON_HPP_
