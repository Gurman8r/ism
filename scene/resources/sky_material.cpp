#include <scene/resources/sky_material.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(SkyMaterial, t) {}

	SkyMaterial::SkyMaterial() {}

	SkyMaterial::~SkyMaterial() {}

	RID SkyMaterial::get_shader_rid() const { return nullptr; }

	ShaderMode_ SkyMaterial::get_shader_mode() const { return ShaderMode_Sky; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}