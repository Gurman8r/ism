#ifndef _ISM_RENDERING_CONTEXT_HPP_
#define _ISM_RENDERING_CONTEXT_HPP_

#include <core/api/class.hpp>

namespace ism
{
	class ISM_API RenderContext : public Super
	{
		ISM_SUPER(RenderContext, Super);

	public:
		virtual ~RenderContext();
	};

	ISM_API_FUNC(RenderContext *) get_render_context();

	ISM_API_FUNC(RenderContext *) set_render_context(RenderContext * value);
}

#endif // !_ISM_RENDERING_CONTEXT_HPP_
