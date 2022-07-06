#include <scene/main/viewport.hpp>

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

	Ref<Image> ViewportTexture::get_image() const
	{
		return {};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Viewport, t, TypeFlags_IsAbstract) {}

	Viewport::Viewport() {}

	Viewport::~Viewport() {}

	Ref<ViewportTexture> Viewport::get_texture() const
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}