#if TOOLS_ENABLED
#ifndef _ISM_EDITOR_TERMINAL_HPP_
#define _ISM_EDITOR_TERMINAL_HPP_

#include <editor/editor_panel.hpp>

namespace Ism
{
	// log component of the main editor
	class ISM_API EditorTerminal : public EditorPanel
	{
		DEFINE_CLASS(EditorTerminal, EditorPanel);

	public:
		EditorTerminal();

		virtual ~EditorTerminal();

	public:
		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_ISM_EDITOR_TERMINAL_HPP_
#endif // TOOLS_ENABLED
