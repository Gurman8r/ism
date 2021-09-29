#include <scene/register_scene_types.hpp>

#include <scene/resources/font.hpp>
#include <scene/resources/material.hpp>
#include <scene/resources/mesh.hpp>
#include <scene/resources/shader.hpp>
#include <scene/resources/sky.hpp>
#include <scene/resources/text_file.hpp>
#include <scene/resources/texture.hpp>

using namespace ism;

void ism::register_scene_types()
{
	api::get_internals()
		.bind_class<Font>()
		.bind_class<Material>()
		.bind_class<Mesh>()
		.bind_class<Shader>()
		.bind_class<Sky>()
		.bind_class<TextFile>()
		.bind_class<Texture>()
		.bind_class<Texture2D>()
		.bind_class<Texture3D>()
		.bind_class<TextureCube>()
		;
}

void ism::unregister_scene_types()
{
}