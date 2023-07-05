#ifndef _ISM_SKY_MATERIAL_HPP_
#define _ISM_SKY_MATERIAL_HPP_

#include <scene/resources/material.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API SkyMaterial : public Material
	{
		DEFINE_CLASS(SkyMaterial, Material);

		friend class MaterialFormatLoader;

	public:
		SkyMaterial();
		virtual ~SkyMaterial() override;
		virtual RID get_shader_rid() const override;
		virtual ShaderMode_ get_shader_mode() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_SKY_MATERIAL_HPP_
