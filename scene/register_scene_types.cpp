#include <scene/register_scene_types.hpp>
#include <scene/main/entity.hpp>
#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>

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
	SINGLETON(Internals)->bind_class<

		SceneTree,
		Node,
		Entity,
		Viewport,
		SubViewport,
		Window,

		Font,
		Material,
		Mesh,
		Shader,
		Sky,
		TextFile,
		Texture,
		Texture2D,
		Texture3D,
		TextureCube

	>();
}

void ism::unregister_scene_types()
{
}