#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/io/image.hpp>
#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingServer : public Object
	{
		OBJECT_COMMON(RenderingServer, Object);

		static RenderingServer * singleton;

	protected:
		static RenderingServer * (*create_func)();

	public:
		explicit RenderingServer() noexcept : Object{} { singleton = this; }

		virtual ~RenderingServer() override {}

		NODISCARD static RenderingServer * get_singleton() noexcept { return singleton; }

		NODISCARD static RenderingServer * create() { return create_func ? create_func() : nullptr; }

	public:
		virtual void initialize() = 0;

		virtual void finalize() = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline RenderingServer & get_rendering_server() noexcept { return *VALIDATE(RenderingServer::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_HPP_
