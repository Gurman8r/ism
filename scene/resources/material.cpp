#include <scene/resources/material_library.hpp>
#include <scene/resources/shader.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Material, t, TypeFlags_IsAbstract) {}

	Material::Material()
	{
		m_material = RENDERING_SERVER->material_create();
	}

	Material::~Material()
	{
		if (m_material) { RENDERING_SERVER->material_destroy(m_material); m_material = nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ShaderMaterial, t) {}

	ShaderMaterial::ShaderMaterial() : Material{}
	{
	}

	ShaderMaterial::ShaderMaterial(Path const & path) : Material{}
	{
		ASSERT(MaterialLibrary::load_material(*this, path) == Error_OK);
	}

	ShaderMaterial::~ShaderMaterial()
	{
	}

	RID ShaderMaterial::get_shader_rid() const { return m_shader ? m_shader->get_rid() : nullptr; }

	Shader::Mode_ ShaderMaterial::get_shader_mode() const { return m_shader->get_mode(); }

	Ref<Shader> ShaderMaterial::get_shader() const { return m_shader; }

	void ShaderMaterial::set_shader(Ref<Shader> const & value)
	{
		if (m_shader == value) { return; }
		m_shader = value;
		RENDERING_SERVER->material_set_shader(get_rid(), get_shader_rid());
	}

	Var ShaderMaterial::get_shader_param(StringName const & key) const
	{
		return RENDERING_SERVER->material_get_param(get_rid(), key);
	}

	void ShaderMaterial::set_shader_param(StringName const & key, Var const & value)
	{
		RENDERING_SERVER->material_set_param(get_rid(), key, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(StandardMaterial3D, t) {}

	StandardMaterial3D::StandardMaterial3D() : Material{}
	{
	}

	StandardMaterial3D::StandardMaterial3D(Path const & path) : Material{}
	{
		ASSERT(MaterialLibrary::load_material(*this, path) == Error_OK);
	}

	StandardMaterial3D::~StandardMaterial3D()
	{
	}

	RID StandardMaterial3D::get_shader_rid() const
	{
		return nullptr;
	}

	Shader::Mode_ StandardMaterial3D::get_shader_mode() const { return Shader::Mode_MAX; }

	void StandardMaterial3D::_update_shader()
	{
	}

	void StandardMaterial3D::set_albedo(Color const & value)
	{
		m_params.albedo = value;
		RENDERING_SERVER->material_set_param(get_rid(), parameter_names[Param_Albedo], (Vec4)value);
	}

	void StandardMaterial3D::set_specular(f32 value)
	{
		m_params.specular = value;
		RENDERING_SERVER->material_set_param(get_rid(), parameter_names[Param_Specular], value);
	}

	void StandardMaterial3D::set_metallic(f32 value)
	{
		m_params.metallic = value;
		RENDERING_SERVER->material_set_param(get_rid(), parameter_names[Param_Metallic], value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}