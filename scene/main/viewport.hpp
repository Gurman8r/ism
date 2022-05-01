#ifndef _ISM_VIEWPORT_HPP_
#define _ISM_VIEWPORT_HPP_

#include <scene/main/node.hpp>
#include <servers/display_server.hpp>
#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Viewport : public Node
	{
		OBJECT_COMMON(Viewport, Node);

		RID m_viewport;

	protected:
		Viewport();

	public:
		virtual ~Viewport() override;

		NODISCARD inline IntRect get_bounds() const noexcept { return { get_position(), get_size() }; }

		NODISCARD virtual Vec2i get_position() const = 0;

		NODISCARD virtual Vec2i get_size() const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_VIEWPORT_HPP_
