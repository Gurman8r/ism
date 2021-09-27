#ifndef _ISM_RENDERING_CONTEXT_HPP_
#define _ISM_RENDERING_CONTEXT_HPP_

#include <core/io/image.hpp>
#include <runtime/renderer/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingContext : public Object
	{
		OBJECT_COMMON(RenderingContext, Object);

		static RenderingContext * singleton;

	protected:
		static RenderingContext * (*create_func)();

	public:
		explicit RenderingContext() : Object{} { singleton = this; }

		virtual ~RenderingContext() override {}

		NODISCARD static RenderingContext * get_singleton() noexcept { return singleton; }

		NODISCARD static RenderingContext * create() { return create_func ? create_func() : nullptr; }

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

	NODISCARD inline RenderingContext & get_rendering_context() noexcept { return *CHECK(RenderingContext::get_singleton()); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_CONTEXT_HPP_
