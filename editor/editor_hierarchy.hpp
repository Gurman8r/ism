#ifndef _ISM_EDITOR_HIERARCHY_HPP_
#define _ISM_EDITOR_HIERARCHY_HPP_

#include <editor/editor_panel.hpp>

namespace ism
{
	class ISM_API EditorHierarchy : public EditorPanel
	{
		OBJECT_COMMON(EditorHierarchy, EditorPanel);

	public:
		EditorHierarchy();

		virtual ~EditorHierarchy();

	public:
		virtual void process(Duration const dt) override;
	};
}

#endif // !_ISM_EDITOR_HIERARCHY_HPP_
