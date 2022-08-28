#ifndef _ISM_EDITOR_LOG_HPP_
#define _ISM_EDITOR_LOG_HPP_

#include <editor/editor_panel.hpp>

namespace ism
{
	// log component of the main editor
	class ISM_API EditorLog : public EditorPanel
	{
		OBJECT_CLASS(EditorLog, EditorPanel);

	public:
		EditorLog();

		virtual ~EditorLog();

	public:
		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_ISM_EDITOR_LOG_HPP_
