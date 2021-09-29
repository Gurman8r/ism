#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// viewport
	class ISM_API Viewport : public Object
	{
		OBJECT_COMMON(Viewport, Object);

	public:
		virtual ~Viewport() override;

		NODISCARD inline Rect get_bounds() const noexcept { return Rect{ get_position(), get_size() }; }

		NODISCARD virtual Vec2 get_position() const = 0;

		NODISCARD virtual Vec2 get_size() const = 0;

		virtual void set_position(Vec2 const & value) = 0;

		virtual void set_size(Vec2 const & value) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VIEWPORT_HPP_
