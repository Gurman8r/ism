#ifndef _ISM_RENDERING_SERVER_DEFAULT_HPP_
#define _ISM_RENDERING_SERVER_DEFAULT_HPP_

#include <runtime/renderer/rendering_context.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingContextDefault : public RenderingContext
	{
		OBJECT_COMMON(RenderingContextDefault, RenderingContext);

	public:
		explicit RenderingContextDefault();

		virtual ~RenderingContextDefault() override;

	public:
		virtual void initialize() override;

		virtual void finalize() override;

	public:
		NODISCARD virtual RID texture2d_create(Ref<Image> const & image) override;

	public:
		NODISCARD virtual RID shader_create() override;

	public:
		NODISCARD virtual RID material_create() override;

	public:
		NODISCARD virtual RID mesh_create() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_DEFAULT_HPP_
