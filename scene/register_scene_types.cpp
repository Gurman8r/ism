#include <scene/register_scene_types.hpp>
#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>
#include <scene/components/script_component.hpp>

#include <scene/resources/font.hpp>
#include <scene/resources/material.hpp>
#include <scene/resources/mesh.hpp>
#include <scene/resources/shader.hpp>
#include <scene/resources/text_file.hpp>
#include <scene/resources/texture.hpp>

using namespace ism;

void ism::register_scene_types()
{
	INIT_OBJECT_CLASS
	(
		SceneTree,
		Node,
		Window,
		Entity,

		Component,
		BehaviorComponent,
		ScriptComponent,

		Font,
		Material,
		Mesh,
		Shader,
		TextFile,
		Texture,
		Texture2D,
		Texture3D,
		Cubemap
	);
}

void ism::unregister_scene_types()
{
}