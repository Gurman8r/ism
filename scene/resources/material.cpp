#include <scene/resources/material_library.hpp>
#include <scene/resources/shader.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Material, t, TypeFlags_IsAbstract) {}

	Material::Material()
	{
		m_material = rendering_server()->material_create();
	}

	Material::~Material()
	{
		if (m_material) { rendering_server()->material_destroy(m_material); m_material = nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ShaderMaterial, t) {}

	ShaderMaterial::ShaderMaterial() : Material{}
	{
	}

	ShaderMaterial::~ShaderMaterial()
	{
	}

	RID ShaderMaterial::get_shader_rid() const { return m_shader ? m_shader->get_rid() : nullptr; }

	ShaderMode_ ShaderMaterial::get_shader_mode() const { return m_shader->get_mode(); }

	Ref<Shader> ShaderMaterial::get_shader() const { return m_shader; }

	void ShaderMaterial::set_shader(Ref<Shader> const & value)
	{
		if (m_shader == value) { return; }
		m_shader = value;
		rendering_server()->material_set_shader(get_rid(), get_shader_rid());
	}

	Var ShaderMaterial::get_shader_param(String const & key) const
	{
		return rendering_server()->material_get_param(get_rid(), key);
	}

	void ShaderMaterial::set_shader_param(String const & key, Var const & value)
	{
		rendering_server()->material_set_param(get_rid(), key, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(StandardMaterial3D, t) {}

	StandardMaterial3D::StandardMaterial3D() : Material{}
	{
	}

	StandardMaterial3D::~StandardMaterial3D()
	{
	}

	RID StandardMaterial3D::get_shader_rid() const
	{
		return nullptr;
	}

	ShaderMode_ StandardMaterial3D::get_shader_mode() const { return ShaderMode_MAX; }

	void StandardMaterial3D::_update_shader()
	{
	}

	void StandardMaterial3D::set_albedo(Color const & value)
	{
		m_params.albedo = value;
		rendering_server()->material_set_param(get_rid(), parameter_names[Param_Albedo], (Vec4)value);
	}

	void StandardMaterial3D::set_specular(f32 value)
	{
		m_params.specular = value;
		rendering_server()->material_set_param(get_rid(), parameter_names[Param_Specular], value);
	}

	void StandardMaterial3D::set_metallic(f32 value)
	{
		m_params.metallic = value;
		rendering_server()->material_set_param(get_rid(), parameter_names[Param_Metallic], value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}