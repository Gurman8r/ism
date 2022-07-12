#include <scene/resources/material_loader.hpp>
#include <scene/resources/shader.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Material, t, TypeFlags_IsAbstract) {}

	Material::Material()
	{
		m_material = RS::get_singleton()->material_create();
	}

	Material::~Material()
	{
		if (m_material) { RS::get_singleton()->material_destroy(m_material); m_material = nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ShaderMaterial, t) {}

	ShaderMaterial::ShaderMaterial() : Material{} {}

	ShaderMaterial::~ShaderMaterial() {}

	Error_ ShaderMaterial::reload_from_file()
	{
		return MaterialLoader::load_from_file(*this, get_path());
	}

	RID ShaderMaterial::get_shader_rid() const { return m_shader ? m_shader->get_rid() : nullptr; }

	Shader::Mode_ ShaderMaterial::get_shader_mode() const { return m_shader->get_mode(); }

	Ref<Shader> ShaderMaterial::get_shader() const { return m_shader; }

	void ShaderMaterial::set_shader(Ref<Shader> const & value)
	{
		if (m_shader == value) { return; }
		m_shader = value;
		RS::get_singleton()->material_set_shader(get_rid(), get_shader_rid());
	}

	Variant ShaderMaterial::get_shader_param(StringName const & key) const
	{
		return RS::get_singleton()->material_get_param(get_rid(), key);
	}

	void ShaderMaterial::set_shader_param(StringName const & key, Variant const & value)
	{
		RS::get_singleton()->material_set_param(get_rid(), key, value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(BaseMaterial3D, t) {}

	BaseMaterial3D::BaseMaterial3D() : Material{} {}

	BaseMaterial3D::~BaseMaterial3D() {}

	Error_ BaseMaterial3D::reload_from_file()
	{
		return MaterialLoader::load_from_file(*this, get_path());
	}

	RID BaseMaterial3D::get_shader_rid() const
	{
		return nullptr;
	}

	Shader::Mode_ BaseMaterial3D::get_shader_mode() const { return Shader::Mode_MAX; }

	void BaseMaterial3D::_update_shader()
	{
	}

	void BaseMaterial3D::set_albedo(Color const & value)
	{
		m_params.albedo = value;
		RS::get_singleton()->material_set_param(get_rid(), parameter_names[Param_Albedo], (Vec4)value);
	}

	void BaseMaterial3D::set_specular(f32 value)
	{
		m_params.specular = value;
		RS::get_singleton()->material_set_param(get_rid(), parameter_names[Param_Specular], value);
	}

	void BaseMaterial3D::set_metallic(f32 value)
	{
		m_params.metallic = value;
		RS::get_singleton()->material_set_param(get_rid(), parameter_names[Param_Metallic], value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}