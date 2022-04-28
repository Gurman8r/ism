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

namespace ism
{
	// base class for editor windows
	class ISM_API EditorPanel : public Object
	{
		OBJECT_COMMON(EditorPanel, Object);

	protected:
		cstring				m_name{};
		bool				m_open{};
		ImGuiWindowFlags	m_flags{};
		ImGuiWindow *		m_window{};

	protected:
		EditorPanel(cstring name, bool start_open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
			: m_name	{ name }
			, m_open	{ start_open }
			, m_flags	{ flags }
		{
		}

		EditorPanel(String const & name, bool start_open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
			: EditorPanel{ name.c_str(), start_open, flags } {}

	public:
		virtual ~EditorPanel();

	public:
		auto get_name() const -> cstring { return m_name; }
		void set_name(cstring value) { m_name = value; }

		auto get_flags() const -> ImGuiWindowFlags { return m_flags; }
		void set_flags(ImGuiWindowFlags value) { m_flags = value; }
		
		bool get_is_open() const { return m_open; }
		void set_is_open(bool value) { m_open = value; }
		void open() { set_is_open(true); }
		void close() noexcept { set_is_open(false); }

	public:
		auto get_window() const noexcept -> ImGuiWindow & { return *VALIDATE(m_window); }
		auto get_draw_list() const noexcept -> ImDrawList * { return get_window().DrawList; }
		auto get_inner_rect() const noexcept -> ImRect { return get_window().InnerRect; }

		auto get_id(cstring str, cstring str_end = NULL) noexcept -> ImGuiID { return get_window().GetID(str, str_end); }
		auto get_id(void const * ptr) noexcept -> ImGuiID { return get_window().GetID(ptr); }
		auto get_id(int32_t n) noexcept -> ImGuiID { return get_window().GetID(n); }
		auto get_id_no_keep_alive(cstring str, cstring str_end = NULL) noexcept -> ImGuiID { return get_window().GetIDNoKeepAlive(str, str_end); }
		auto get_id_no_keep_alive(void const * ptr) noexcept -> ImGuiID { return get_window().GetIDNoKeepAlive(ptr); }
		auto get_id_no_keep_alive(int32_t n) noexcept -> ImGuiID { return get_window().GetIDNoKeepAlive(n); }
		auto get_id_from_rectangle(ImRect const & r) noexcept -> ImGuiID { return get_window().GetIDFromRectangle(r); }
		
		auto get_rect() const noexcept -> ImRect { return get_window().Rect(); }
		auto get_font_size() const -> float_t { return get_window().CalcFontSize(); }
		auto get_title_bar_height() const -> float_t { return get_window().TitleBarHeight(); }
		auto get_title_bar_rect() const noexcept -> ImRect { return get_window().TitleBarRect(); }
		auto get_menu_bar_height() const -> float_t { return get_window().MenuBarHeight(); }
		auto get_menu_bar_rect() const noexcept -> ImRect { return get_window().MenuBarRect(); }
		
	public:
		virtual void draw() {}

	protected:
		bool begin_window();

		void end_window();

		void update() noexcept;

		virtual void do_update(ImGuiWindow * window);
	};
}

#endif // !_ISM_EDITOR_PANEL_HPP_
