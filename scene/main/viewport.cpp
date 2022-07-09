#include <scene/main/viewport.hpp>

// viewport texture
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ViewportTexture, t) {}

	ViewportTexture::ViewportTexture()
	{
	}

	ViewportTexture::~ViewportTexture()
	{
	}

	RID ViewportTexture::get_rid() const
	{
		return RID();
	}

	int32_t ViewportTexture::get_width() const
	{
		return 0;
	}

	int32_t ViewportTexture::get_height() const
	{
		return 0;
	}

	Vec2i ViewportTexture::get_size() const
	{
		return {};
	}

	bool ViewportTexture::has_alpha() const
	{
		return false;
	}

	Ref<Image> ViewportTexture::get_data() const
	{
		return {};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// viewport
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Viewport, t, TypeFlags_IsAbstract) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Viewport::Viewport() {}

	Viewport::~Viewport() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Viewport::process(Duration const & dt)
	{
		base_type::process(dt);
	}

	void Viewport::notification(int32_t id)
	{
		base_type::notification(id);
		switch (id)
		{
		case Notification_EnterTree: {
		} break;
		case Notification_ExitTree: {
		} break;
		case Notification_Ready: {
		} break;
		case Notification_InternalUpdate: {
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vec2i Viewport::_get_size() const
	{
		return Vec2i();
	}

	void Viewport::_set_size(Vec2i const & size)
	{
	}

	Ref<ViewportTexture> Viewport::get_texture() const
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}