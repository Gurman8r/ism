#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingServer : public Object
	{
		ISM_OBJECT(RenderingServer, Object);

		static RenderingServer * singleton;

	protected:
		explicit RenderingServer() noexcept : Object{} { singleton = this; }

	public:
		virtual ~RenderingServer() override { singleton = nullptr; }

		NODISCARD static RenderingServer * get_singleton() noexcept { return singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline RenderingServer & get_rendering_server() { return *RenderingServer::get_singleton(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_HPP_
