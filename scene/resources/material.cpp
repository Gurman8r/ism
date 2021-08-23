#include <scene/resources/material.hpp>

using namespace ism;

ISM_OBJECT_IMPLEMENTATION(Material, t, "material")
{
	t.tp_base = typeof<Resource>();
}

Material::~Material()
{
}

Material::Material() : Material{ get_class() }
{
}