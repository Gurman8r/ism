#include <scene/resources/shader_loader.hpp>
#include <servers/rendering_server.hpp>
#include <core/io/filesystem.hpp>

#if DIRECTX_ENABLED
#include <drivers/directx/shader_loader_hlsl.hpp>
#endif

#if OPENGL_ENABLED
#include <drivers/opengl/shader_loader_glsl.hpp>
#endif

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_shader(Shader & shader, Path const & path)
{
	if (!path) { return Error_Unknown; }

	// parse json file
	std::ifstream file{ path.c_str() };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return Error_Unknown; }
	JSON json{ JSON::parse(file) };
	if (json.empty()) { return Error_Unknown; }

	// generate spec
	RD::ShaderCreateInfo shader_spec;
	RD::ShaderLanguage_ shader_language;
	auto lang{ json.find("lang") };
	ASSERT(lang != json.end());
	switch (hash(lang->get<String>())) {
	default: { return Error_Unknown; } break;
	case "GLSL"_hash: {
#if OPENGL_ENABLED
		shader_spec = ShaderLoaderGLSL::create_shader_spec(json);
#else
		CRASH("OPENGL IS NOT ENABLED");
#endif
	} break;
	case "HLSL"_hash: {
#if DIRECTX_ENABLED
		shader_spec = ShaderLoaderHLSL::create_shader_spec(json);
#else
		CRASH("DIRECTX IS NOT ENABLED");
#endif
	} break;
	}

	// create shader
	if (shader.m_shader) { RENDERING_DEVICE->shader_destroy(shader.m_shader); }
	shader.m_shader = RENDERING_DEVICE->shader_create(shader_spec);
	if (!shader.m_shader) { return Error_Unknown; }

	return Error_None;
}

Error_ ShaderLoader::load_shader(Ref<Shader> shader, Path const & path)
{
	if (!shader) { return Error_Unknown; }

	return load_shader(**shader, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_shader(Shader & shader, DynamicBuffer const & code)
{
	// TODO: load shader binary

	return Error_None;
}

Error_ ShaderLoader::load_shader(Ref<Shader> shader, DynamicBuffer const & code)
{
	if (!shader) { return Error_Unknown; }

	return load_shader(**shader, code);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */