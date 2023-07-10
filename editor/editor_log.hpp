#if TOOLS_ENABLED
#ifndef _ISM_EDITOR_LOG_HPP_
#define _ISM_EDITOR_LOG_HPP_

#include <editor/editor_panel.hpp>

namespace Ism
{
	// log component of the main editor
	class ISM_API EditorLog : public EditorPanel
	{
		OBJECT_CLASS(EditorLog, EditorPanel);

	public:
		EditorLog();

		virtual ~EditorLog();

	public:
		virtual void process(Duration const & dt) override;
	};
}

#endif // !_ISM_EDITOR_LOG_HPP_
#endif // TOOLS_ENABLED
