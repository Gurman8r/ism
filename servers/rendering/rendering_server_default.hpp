#ifndef _ISM_RENDERING_SERVER_DEFAULT_HPP_
#define _ISM_RENDERING_SERVER_DEFAULT_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RenderingServerDefault : public RenderingServer
	{
		OBJECT_COMMON(RenderingServerDefault, RenderingServer);

		Ref<RenderingDevice> m_device{};

	public:
		RenderingServerDefault();

		virtual ~RenderingServerDefault() override;

		virtual void initialize() override;

		virtual void finalize() override;

	public:
		/* TEXTURE */
		virtual RID texture_replace(RID old_texture, RID new_texture) override;
		virtual RID texture2d_create(Ref<Image> const & image) override;
		virtual RID texture2d_placeholder_create() override;
		virtual Ref<Image> texture2d_get_image(RID texture) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERING_SERVER_DEFAULT_HPP_
