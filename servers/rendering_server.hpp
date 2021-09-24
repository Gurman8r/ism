#ifndef _ISM_RENDERING_SERVER_HPP_
#define _ISM_RENDERING_SERVER_HPP_

#include <core/io/image.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingServer : public Object
	{
		OBJECT_CLASS(RenderingServer, Object);

		static RenderingServer * singleton;

	protected:
		static RenderingServer * (*create_func)();

	public:
		explicit RenderingServer() : Object{} { singleton = this; }

		virtual ~RenderingServer() override {}

		NODISCARD static RenderingServer * get_singleton() noexcept { return singleton; }

		NODISCARD static RenderingServer * create() { return create_func ? create_func() : nullptr; }

	public:
		virtual void initialize() = 0;

		virtual void finalize() = 0;

	public:
		NODISCARD virtual RID texture2d_create(Ref<Image> const & image) = 0;

	public:
		NODISCARD virtual RID shader_create() = 0;

	public:
		NODISCARD virtual RID material_create() = 0;

	public:
		NODISCARD virtual RID mesh_create() = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline RenderingServer & get_rendering_server() { return *CHECK(RenderingServer::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_HPP_
