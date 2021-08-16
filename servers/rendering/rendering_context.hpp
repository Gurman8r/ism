#ifndef _ISM_RENDERING_CONTEXT_HPP_
#define _ISM_RENDERING_CONTEXT_HPP_

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// rendering context
	class ISM_API RenderingContext : public Super
	{
		ISM_SUPER(RenderingContext, Super);

	public:
		virtual ~RenderingContext();
	};

	ISM_API_FUNC(RenderingContext *) get_render_context();

	ISM_API_FUNC(RenderingContext *) set_render_context(RenderingContext * value);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_CONTEXT_HPP_
