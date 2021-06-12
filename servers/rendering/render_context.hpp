#ifndef _ISM_RENDERING_CONTEXT_HPP_
#define _ISM_RENDERING_CONTEXT_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API RenderContext : public Super
	{
		ISM_SUPER_CLASS(RenderContext, Super);

	public:
		virtual ~RenderContext() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(RenderContext);
	};
}

#endif // !_ISM_RENDERING_CONTEXT_HPP_
