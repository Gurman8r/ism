#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API RenderingServer : public Super
	{
		ISM_SUPER_CLASS(RenderingServer, Super);

	public:
		virtual ~RenderingServer() override;

		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(RenderingServer);
	};
}

#endif // !_ISM_RENDERING_SERVER_HPP_
