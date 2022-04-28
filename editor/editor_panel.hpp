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

		cstring				m_name{};
		bool				m_is_open{};
		ImGuiWindowFlags	m_flags{};
		ImGuiWindow *		m_window{};

	protected:
		EditorPanel(cstring name, bool start_open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
			: m_name	{ name }
			, m_is_open	{ start_open }
			, m_flags	{ flags }
		{
		}

		EditorPanel(String const & name, bool start_open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
			: EditorPanel{ name.c_str(), start_open, flags } {}

	public:
		virtual ~EditorPanel();

	public:
		auto get_name() const noexcept -> cstring { return m_name; }
		void set_name(cstring value) noexcept { m_name = value; }
		
		bool is_open() const noexcept { return m_is_open; }
		void set_open(bool value) noexcept { m_is_open = value; }
		void open() noexcept { m_is_open = true; }
		void close() noexcept { m_is_open = false; }
		void toggle_open() noexcept { m_is_open = !m_is_open; }

		auto get_flags() const noexcept -> ImGuiWindowFlags { return m_flags; }
		void set_flags(ImGuiWindowFlags value) noexcept { m_flags = value; }

	public:
		auto get_window() const noexcept -> ImGuiWindow * { return m_window; }
		auto operator->() const noexcept -> ImGuiWindow * { return m_window; }

		auto get_id(cstring str, cstring str_end = NULL) -> ImGuiID { return m_window->GetID(str, str_end); }
		auto get_id(void const * ptr) -> ImGuiID { return m_window->GetID(ptr); }
		auto get_id(int32_t n) -> ImGuiID { return m_window->GetID(n); }
		auto get_id_no_keep_alive(cstring str, cstring str_end = NULL) -> ImGuiID { return m_window->GetIDNoKeepAlive(str, str_end); }
		auto get_id_no_keep_alive(void const * ptr) -> ImGuiID { return m_window->GetIDNoKeepAlive(ptr); }
		auto get_id_no_keep_alive(int32_t n) -> ImGuiID { return m_window->GetIDNoKeepAlive(n); }
		auto get_id_from_rectangle(ImRect const & r) -> ImGuiID { return m_window->GetIDFromRectangle(r); }
		
		auto get_rect() const -> ImRect { return m_window->Rect(); }
		auto get_font_size() const -> float_t { return m_window->CalcFontSize(); }
		auto get_title_bar_height() const -> float_t { return m_window->TitleBarHeight(); }
		auto get_title_bar_rect() const -> ImRect { return m_window->TitleBarRect(); }
		auto get_menu_bar_height() const -> float_t { return m_window->MenuBarHeight(); }
		auto get_menu_bar_rect() const -> ImRect { return m_window->MenuBarRect(); }
		
	public:
		virtual void draw() = 0;

	protected:
		virtual bool begin_window();

		virtual void end_window();
	};
}

#endif // !_ISM_EDITOR_PANEL_HPP_
