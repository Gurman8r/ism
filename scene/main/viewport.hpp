#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>
#include <scene/resources/texture.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class CameraBehavior;
	class Window;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// viewport texture
	class ViewportTexture : public Texture2D {
		OBJECT_CLASS(ViewportTexture, Texture2D);
	public:
		ViewportTexture();
		virtual ~ViewportTexture() override;
		virtual RID get_rid() const override;
		virtual i32 get_width() const override;
		virtual i32 get_height() const override;
		virtual Vec2i get_size() const override;
		virtual bool has_alpha() const override;
		virtual Ref<Image> get_data() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// viewport
	class ISM_API Viewport : public Node
	{
		OBJECT_CLASS(Viewport, Node);

		friend class Window;

		Viewport * m_parent{};
		Viewport * m_gui_parent{};

		CameraBehavior * m_main_camera{};

		RID m_viewport{};
		RID m_current_canvas{};
		RID m_subwindow_canvas{};

		RID m_texture_rid{};
		Ref<ViewportTexture> m_default_texture{};
		Set<ViewportTexture *> m_viewport_textures{};

		enum SubWindowDrag_ {
			SubWindowDrag_Disabled,
			SubWindowDrag_Move,
			SubWindowDrag_Clost,
			SubWindowDrag_Resize,
		};

		enum SubWindowResize_ {
			SubWindowResize_Disabled,
			SubWindowResize_TopLeft,
			SubWindowResize_Top,
			SubWindowResize_TopRight,
			SubWindowResize_Left,
			SubWindowResize_Right,
			SubWindowResize_BottomLeft,
			SubWindowResize_Bottom,
			SubWindowResize_BottomRight,
			SubWindowResize_MAX
		};

		struct SubWindow
		{
			Window * window{};
			RID canvas_item{};
		};

		struct GUI
		{
			Vector<SubWindow> subwindows{};
		}
		m_gui{};

	public:
		Viewport();
		virtual ~Viewport() override;

	protected:
		virtual Vec2i _get_size() const;
		virtual void _set_size(Vec2i const & size);

	public:
		Ref<ViewportTexture> get_texture() const;
		virtual DS::WindowID get_window_id() const = 0;
		virtual RID get_viewport_rid() const { return m_viewport; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VIEWPORT_HPP_
