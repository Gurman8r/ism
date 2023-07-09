#include <scene/register_scene_types.hpp>
#include <core/extension/extension_manager.hpp>

#include <scene/main/scene_tree.hpp>
#include <scene/main/window.hpp>
#include <scene/main/missing_node.hpp>
#include <scene/main/entity.hpp>

#include <scene/resources/font_library.hpp>
#include <scene/resources/material_library.hpp>
#include <scene/resources/mesh_library.hpp>
#include <scene/resources/shader_library.hpp>
#include <scene/resources/text_file.hpp>
#include <scene/resources/texture.hpp>

#include <scene/resources/sky_material.hpp>

namespace Ism
{
	static Ref<ShaderFormatLoader> shader_loader;
	static Ref<MaterialFormatLoader> material_loader;
	static Ref<MeshFormatLoader> mesh_loader;
	static Ref<FontFormatLoader> font_loader;

	void register_scene_types()
	{
		font_loader.instance(); get_loaders()->add_resource_format_loader(font_loader);
		material_loader.instance(); get_loaders()->add_resource_format_loader(material_loader);
		mesh_loader.instance(); get_loaders()->add_resource_format_loader(mesh_loader);
		shader_loader.instance(); get_loaders()->add_resource_format_loader(shader_loader);

		REGISTER_CLASS
		(
			SceneTree, Node, MissingNode, Entity,
			
			Viewport, Window,

			Font, TextFile,

			Shader, Material, ShaderMaterial, StandardMaterial3D,
			
			Mesh,

			Texture, Texture2D, Texture3D, TextureCube,

			SkyMaterial
		);
	}

	void register_scene_singletons()
	{
	}
	
	void unregister_scene_types()
	{
		get_loaders()->remove_resource_format_loader(font_loader); font_loader = nullptr;
		get_loaders()->remove_resource_format_loader(mesh_loader); mesh_loader = nullptr;
		get_loaders()->remove_resource_format_loader(material_loader); material_loader = nullptr;
		get_loaders()->remove_resource_format_loader(shader_loader); shader_loader = nullptr;
	}
}