#include <scene/resources/material.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Material, t, TypeFlags_IsAbstract) {}

Material::Material()
{
	m_material = RENDERING_SERVER->material_create();
}

Material::~Material()
{
	if (m_material) { RENDERING_SERVER->material_destroy(m_material); m_material = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(BaseMaterial3D, t) {}

BaseMaterial3D::BaseMaterial3D() : Material{}
{
}

BaseMaterial3D::~BaseMaterial3D()
{
}

RID BaseMaterial3D::get_shader_rid() const
{
	return nullptr;
}

Error_ BaseMaterial3D::reload_from_file()
{
	if (!get_path()) { return Error_Unknown; }

	return Error_None;
}

void BaseMaterial3D::_update_shader()
{
}

void BaseMaterial3D::set_albedo(Color const & value)
{
	m_params.albedo = value;
	RENDERING_SERVER->material_set_param(get_rid(), parameter_names[Param_Albedo], nullptr);
}

void BaseMaterial3D::set_specular(float_t value)
{
	m_params.specular = value;
	RENDERING_SERVER->material_set_param(get_rid(), parameter_names[Param_Specular], nullptr);
}

void BaseMaterial3D::set_metallic(float_t value)
{
	m_params.metallic = value;
	RENDERING_SERVER->material_set_param(get_rid(), parameter_names[Param_Metallic], nullptr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */