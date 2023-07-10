#include <scene/register_scene_types.hpp>
#include <core/extension/extension_manager.hpp>

#include <scene/main/scene_tree.hpp>
#include <scene/main/window.hpp>
#include <scene/main/missing_node.hpp>
#include <scene/main/instance_placeholder.hpp>
#include <scene/main/resource_preloader.hpp>
#include <scene/main/entity.hpp>

#include <scene/resources/font_library.hpp>
#include <scene/resources/material_library.hpp>
#include <scene/resources/mesh_library.hpp>
#include <scene/resources/shader_library.hpp>
#include <scene/resources/text_file.hpp>

#include <scene/resources/texture.hpp>

#include <scene/resources/scene.hpp>
#include <scene/resources/world_2d.hpp>
#include <scene/resources/world_3d.hpp>

namespace Ism
{
	static Ref<ShaderFormatLoader> shader_loader;
	static Ref<MaterialFormatLoader> material_loader;
	static Ref<MeshFormatLoader> mesh_loader;
	static Ref<FontFormatLoader> font_loader;

	void register_scene_types()
	{
		resource_loader()->add_resource_format_loader(font_loader.instance());
		resource_loader()->add_resource_format_loader(material_loader.instance());
		resource_loader()->add_resource_format_loader(mesh_loader.instance());
		resource_loader()->add_resource_format_loader(shader_loader.instance());

		REGISTER_CLASS
		(
			SceneTree, Node, MissingNode, InstancePlaceholder, Entity,
			
			Viewport, Window,

			Font,

			Shader, Material, ShaderMaterial, StandardMaterial3D,
			
			Mesh,

			Texture, Texture2D, Texture3D, TextureCube,

			World2D, World3D,

			TextFile
		);
	}

	void register_scene_singletons()
	{
	}
	
	void unregister_scene_types()
	{
		resource_loader()->remove_resource_format_loader(font_loader); font_loader = nullptr;
		resource_loader()->remove_resource_format_loader(mesh_loader); mesh_loader = nullptr;
		resource_loader()->remove_resource_format_loader(material_loader); material_loader = nullptr;
		resource_loader()->remove_resource_format_loader(shader_loader); shader_loader = nullptr;
	}
}