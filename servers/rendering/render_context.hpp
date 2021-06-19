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

		DEFAULT_COPYABLE_MOVABLE(RenderContext);
	};
}

#endif // !_ISM_RENDERING_CONTEXT_HPP_
