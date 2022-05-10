#include <scene/resources/material.hpp>
#include <servers/rendering/renderer_storage.hpp>

using namespace ism;

OBJECT_EMBED(Material, t) {}

Material::Material() : Resource{}
{
	m_material = RENDERER_STORAGE->material_create();
}

Material::~Material()
{
	if (m_material) { RENDERER_STORAGE->material_destroy(m_material); m_material = nullptr; }
}