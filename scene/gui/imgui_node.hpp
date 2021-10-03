#ifndef _ISM_IMGUI_NODE_HPP_
#define _ISM_IMGUI_NODE_HPP_

#include <scene/main/scene_tree.hpp>
#include <scene/gui/imgui.hpp>

namespace ism
{
	class ISM_API ImGuiNode : public Node
	{
		OBJECT_COMMON(ImGuiNode, Node);

		ImGuiContext * m_imgui_context{};

	public:
		ImGuiNode(cstring log_filename = "", cstring ini_filename = "");

		~ImGuiNode();

		virtual void begin_step() override;

		virtual void step(Duration const &) override;

		virtual void end_step() override;

	public:
		void begin_gui_frame();

		void end_gui_frame();

	public:
		NODISCARD auto get_context() const noexcept -> ImGuiContext & { return *VALIDATE(m_imgui_context); }

		NODISCARD auto get_io() const noexcept -> ImGuiIO & { return get_context().IO; }

		NODISCARD auto get_platform_io() const noexcept -> ImGuiPlatformIO & { return get_context().PlatformIO; }

		NODISCARD auto get_style() const noexcept -> ImGuiStyle & { return get_context().Style; }

	public:
		NODISCARD auto get_viewports() const noexcept -> ImVector<ImGuiViewportP *> & { return get_context().Viewports; }

		NODISCARD auto get_viewport(size_t const i) const -> ImGuiViewportP * { return get_viewports()[i]; }
	};
}

#endif // !_ISM_IMGUI_NODE_HPP_
