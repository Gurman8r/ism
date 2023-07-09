#if TOOLS_ENABLED
#ifndef _ISM_EDITOR_PANEL_HPP_
#define _ISM_EDITOR_PANEL_HPP_

#include <scene/gui/imgui.hpp>
#include <scene/main/scene_tree.hpp>

#define EDITOR_PANEL_SCOPE(open)							\
	(open) = Ism::EditorPanel::begin_window();				\
	ON_SCOPE_EXIT(&) { Ism::EditorPanel::end_window(); };	\

namespace Ism
{
	// base class for editor windows
	class ISM_API EditorPanel : public Object
	{
		OBJECT_CLASS(EditorPanel, Object);

		ImGuiWindow *		m_window;
		cstring				m_name;
		bool				m_is_open;
		bool				m_is_focused;
		ImGuiWindowFlags	m_flags;

	protected:
		EditorPanel(cstring name, bool start_open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

	public:
		virtual ~EditorPanel();

		virtual void process(Duration const & delta_time) = 0;

	public:
		auto get_window() const noexcept -> ImGuiWindow * { return m_window; }

		auto get_name() const noexcept -> cstring { return m_name; }
		void set_name(cstring value) noexcept { m_name = value; }
		
		bool is_open() const noexcept { return m_is_open; }
		void set_open(bool value) noexcept { m_is_open = value; }
		void toggle_open() noexcept { m_is_open = !m_is_open; }

		bool is_focused() const noexcept { return m_is_focused; }

		auto get_flags() const noexcept -> ImGuiWindowFlags { return m_flags; }
		void set_flags(ImGuiWindowFlags value) noexcept { m_flags = value; }

	protected:
		virtual bool begin_window();

		virtual void end_window();
	};
}

#endif // !_ISM_EDITOR_PANEL_HPP_
#endif // TOOLS_ENABLED
