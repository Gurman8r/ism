#ifndef _ISM_RENDERING_SERVER_DEFAULT_HPP_
#define _ISM_RENDERING_SERVER_DEFAULT_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingServerDefault : public RenderingServer
	{
		OBJECT_CLASS(RenderingServerDefault, RenderingServer);

	public:
		explicit RenderingServerDefault();

		virtual ~RenderingServerDefault() override;

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
