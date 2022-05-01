#ifndef _ISM_EDITOR_PANEL_HPP_
#define _ISM_EDITOR_PANEL_HPP_

#include <scene/gui/imgui.hpp>
#include <scene/main/scene_tree.hpp>
#include <scene/resources/font.hpp>
#include <scene/resources/material.hpp>
#include <scene/resources/mesh.hpp>
#include <scene/resources/shader.hpp>
#include <scene/resources/text_file.hpp>
#include <scene/resources/texture.hpp>

#define EDITOR_PANEL_SCOPE(open)						\
	(open) = ism::EditorPanel::begin_window();			\
	SCOPE_EXIT(&) { ism::EditorPanel::end_window(); };	\

namespace ism
{
	// base class for editor windows
	class ISM_API EditorPanel : public Object
	{
		OBJECT_COMMON(EditorPanel, Object);

		cstring				m_name;
		bool				m_is_open;
		ImGuiWindowFlags	m_flags;
		ImGuiWindow *		m_window;

	protected:
		EditorPanel(cstring name, bool start_open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

	public:
		virtual ~EditorPanel();

		virtual void process(Duration const dt) = 0;

	public:
		auto get_name() const noexcept -> cstring { return m_name; }
		void set_name(cstring value) noexcept { m_name = value; }
		
		bool is_open() const noexcept { return m_is_open; }
		void set_open(bool value) noexcept { m_is_open = value; }
		void toggle_open() noexcept { m_is_open = !m_is_open; }

		auto get_flags() const noexcept -> ImGuiWindowFlags { return m_flags; }
		void set_flags(ImGuiWindowFlags value) noexcept { m_flags = value; }

		auto get_window() const noexcept -> ImGuiWindow * { return m_window; }
		auto operator->() const noexcept -> ImGuiWindow * { return m_window; }

	protected:
		virtual bool begin_window();

		virtual void end_window();
	};
}

#endif // !_ISM_EDITOR_PANEL_HPP_
