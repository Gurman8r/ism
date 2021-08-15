#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/api/class.hpp>

namespace ism
{
	class ISM_API RenderingServer : public Super
	{
		ISM_SUPER(RenderingServer, Super);

		static RenderingServer * singleton;

	public:
		virtual ~RenderingServer();

		static RenderingServer * get_singleton() { return singleton; }
	};
}

#endif // !_ISM_RENDERING_SERVER_HPP_
