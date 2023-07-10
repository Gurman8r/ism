#include <scene/main/viewport.hpp>
#include <scene/component/camera_behavior.hpp>

// viewport texture
namespace Ism
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

	i32 ViewportTexture::get_width() const
	{
		return 0;
	}

	i32 ViewportTexture::get_height() const
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
namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Viewport, t, TypeFlags_IsAbstract) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Viewport::Viewport() {}

	Viewport::~Viewport() {}

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