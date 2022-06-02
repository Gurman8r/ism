#include <scene/resources/shader_loader.hpp>
#include <servers/rendering_server.hpp>
#include <core/io/filesystem.hpp>

#if OPENGL_ENABLED
#include <drivers/opengl/shader_loader_opengl.hpp>
#endif

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_shader(Shader & shader, Path const & path)
{
	if (!path) { return Error_Unknown; }

	if (shader.m_shader) { RENDERING_DEVICE->shader_destroy(shader.m_shader); }

	std::ifstream file{ path.c_str() };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return Error_Unknown; }

	JSON json{ JSON::parse(file) };
	if (json.empty()) { return Error_Unknown; }

	RD::ShaderCreateInfo spec;
	if (auto language{ json.find("language") }; language != json.end())
	{
		switch (hash(util::to_lower(language->get<String>())))
		{
		default: { return Error_Unknown; } break;
		case hash("glsl"): {
#if OPENGL_ENABLED
			spec = ShaderLoaderOpenGL::create_shader_spec(json);
#endif
		} break;
		case hash("hlsl"): {
#if DIRECTX_ENABLED
			// TODO
#endif
		} break;
		}
	}
	else
	{
		return Error_Unknown;
	}
	
	shader.m_shader = RENDERING_DEVICE->shader_create(spec);
	
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
	return Error_None;
}

Error_ ShaderLoader::load_shader(Ref<Shader> shader, DynamicBuffer const & code)
{
	if (!shader) { return Error_Unknown; }

	return load_shader(**shader, code);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */