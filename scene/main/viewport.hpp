#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>

#include <servers/display_server.hpp>

namespace ism
{
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
