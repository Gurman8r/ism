#if TOOLS_ENABLED
#ifndef _ISM_EDITOR_TERMINAL_HPP_
#define _ISM_EDITOR_TERMINAL_HPP_

#include <editor/editor_panel.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// log component of the main editor
	class ISM_API EditorTerminal : public EditorPanel
	{
		OBJECT_CLASS(EditorTerminal, EditorPanel);

		Array<char, 256>	m_input{};
		Vector<String>		m_items{};
		Vector<String>		m_commands{};
		Vector<String>		m_history{};
		i32					m_history_pos{ -1 }; // -1: new line, 0..m_history.Size-1 browsing history.
		ImGuiTextFilter		m_filter{};
		bool				m_autoscroll{ true };
		bool				m_scroll_to_bottom{};

		class EditorTerminalLogger * m_logger{};
		class Script * m_script{};

	public:
		EditorTerminal();
		virtual ~EditorTerminal();
		virtual void process(Duration const & delta_time) override;

		void clear();
		void printv(cstring fmt, va_list args);
		void printf(cstring fmt, ...);
		void execute(String const & command_line);

	protected:
		virtual i32 text_edit_callback(ImGuiInputTextCallbackData * data);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API EditorTerminalLogger : public Logger {
		EditorTerminal * m_terminal{};
	public:
		EditorTerminalLogger(EditorTerminal * terminal) noexcept : m_terminal{ terminal } {}
		virtual void logv(cstring fmt, va_list args, bool is_error = false) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_EDITOR_TERMINAL_HPP_
#endif // TOOLS_ENABLED
