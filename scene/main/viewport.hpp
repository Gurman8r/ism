#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// video mode
	struct NODISCARD VideoMode
	{
		Vec2	size	{ 1280, 720 }	; // resolution
		Vec4	bpp		{ 8, 8, 8, 8 }	; // bits-per-pixel
		int32_t	rate	{ -1 }			; // refresh rate

		DEFAULT_COPYABLE_MOVABLE(VideoMode);
	};

	ISM_API_FUNC(VideoMode const &) get_desktop_video_mode();

	ISM_API_FUNC(Vector<VideoMode> const &) get_fullscreen_video_modes();

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// viewport
	class ISM_API Viewport : public Node
	{
		ISM_SUPER(Viewport, Node);

	protected:
		explicit Viewport(SceneTree * tree, Node * parent) : Node{ tree, parent } {}

		explicit Viewport(SceneTree * tree) : Node{ tree } {}

		explicit Viewport(Node * parent) : Node{ parent } {}

	public:
		virtual ~Viewport() override;

		NODISCARD inline Rect get_bounds() const noexcept { return Rect{ get_position(), get_size() }; }

		NODISCARD virtual Vec2 get_framebuffer_size() const = 0;

		NODISCARD virtual Vec2 get_position() const = 0;

		NODISCARD virtual Vec2 get_size() const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VIEWPORT_HPP_
