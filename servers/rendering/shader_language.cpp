#include <servers/rendering/shader_language.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using Type_ = ShaderBuilder::Type_;

static void test_shader_builder()
{
	ShaderBuilder::StageDef stage{ RD::ShaderStage_Vertex };
	
	stage.inputs = {
		{ "Position", 0, ShaderBuilder::Type_Vec4 },
		{ "Normal", 1, ShaderBuilder::Type_Vec4 },
		{ "UV", 2, ShaderBuilder::Type_Vec4 },
		{ "Tangent", 3, ShaderBuilder::Type_Vec4 },
		{ "Bitangent", 4, ShaderBuilder::Type_Vec4 },
	};

	stage.outputs = {
		{ "Pixel_Position", 0, ShaderBuilder::Type_Vec4 },
		{ "Pixel_Normal", 1, ShaderBuilder::Type_Vec4 },
		{ "Pixel_UV", 2, ShaderBuilder::Type_Vec4 },
		{ "Pixel_Tangent", 3, ShaderBuilder::Type_Vec4 },
		{ "Pixel_Bitangent", 4, ShaderBuilder::Type_Vec4 },
	};

	stage.buffers = {
		{ "SceneState", 0, {
			{ "Projection", 0, ShaderBuilder::Type_Mat4 },
			{ "View", 0, ShaderBuilder::Type_Mat4 },
		} },
		{ "RenderPass", 1, {
			{ "Placeholder", 0, ShaderBuilder::Type_Mat4 },
		} },
		{ "Transforms", 1, {
			{ "Transform", 0, ShaderBuilder::Type_Mat4 },
		} },
		{ "Material", 1, {
			{ "Ambient", 0, ShaderBuilder::Type_Vec4 },
			{ "Diffuse", 1, ShaderBuilder::Type_Vec4 },
			{ "Specular", 2, ShaderBuilder::Type_Vec4 },
			{ "Shininess", 3, ShaderBuilder::Type_Float },
		} },
	};

	stage.functions = {
		{ "test", ShaderBuilder::Type_Void, {}, },
	};

	stage.main = {
		"main", ShaderBuilder::Type_Void, {},
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */