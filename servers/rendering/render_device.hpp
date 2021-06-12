#ifndef _ISM_RENDERING_DEVICE_HPP_
#define _ISM_RENDERING_DEVICE_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API RenderDevice : public Super
	{
		ISM_SUPER_CLASS(RenderDevice, Super);

	public:
		virtual ~RenderDevice() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(RenderDevice);
	};
}

#endif // !_ISM_RENDERING_DEVICE_HPP_
