#ifndef _ISM_PANEL_HPP_
#define _ISM_PANEL_HPP_

#include <scene/gui/gui_node.hpp>

namespace ism
{
	class ISM_API Panel : public GuiNode
	{
		OBJECT_COMMON(Panel, GuiNode);

	public:
		virtual ~Panel();
	};
}

#endif // !_ISM_PANEL_HPP_
