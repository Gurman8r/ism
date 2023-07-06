#if TOOLS_ENABLED
#ifndef _ISM_EDITOR_INSPECTOR_HPP_
#define _ISM_EDITOR_INSPECTOR_HPP_

#include <editor/editor_panel.hpp>

namespace Ism
{
	// log component of the main editor
	class ISM_API EditorInspector : public EditorPanel
	{
		DEFINE_CLASS(EditorInspector, EditorPanel);

	public:
		EditorInspector();

		virtual ~EditorInspector();

	public:
		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_ISM_EDITOR_INSPECTOR_HPP_
#endif // TOOLS_ENABLED
