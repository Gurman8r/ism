#ifndef _ISM_RENDERING_DEVICE_HPP_
#define _ISM_RENDERING_DEVICE_HPP_

#include <core/api/object/generic_object.hpp>

namespace ism
{
	class ISM_API RenderDevice// : public Super
	{
		//ISM_SUPER(RenderDevice, Super);

	public:
		virtual ~RenderDevice();

		DEFAULT_COPYABLE_MOVABLE(RenderDevice);
	};
}

#endif // !_ISM_RENDERING_DEVICE_HPP_
