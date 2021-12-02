#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering/rendering_command.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// viewport
	class ISM_API Viewport : public Node
	{
		OBJECT_COMMON(Viewport, Node);

	protected:
		Viewport();

	public:
		virtual ~Viewport() override;

		virtual void process(Duration const & dt) override;

		NODISCARD inline Rect get_bounds() const noexcept { return Rect{ get_position(), get_size() }; }

		NODISCARD virtual Vec2 get_position() const = 0;

		NODISCARD virtual Vec2 get_size() const = 0;

		virtual void set_position(Vec2 const & value) = 0;

		virtual void set_size(Vec2 const & value) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// sub-viewport
	class ISM_API SubViewport : public Viewport
	{
		OBJECT_COMMON(SubViewport, Viewport);

	public:
		SubViewport();

		virtual ~SubViewport() override;

		virtual void process(Duration const & dt) override;

		using Viewport::get_bounds;

		using Viewport::get_position;

		using Viewport::get_size;

		using Viewport::set_position;

		using Viewport::set_size;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VIEWPORT_HPP_
