#include <scene/resources/shader_loader.hpp>
#include <servers/rendering_server.hpp>
#include <core/io/file_system.hpp>

#if OPENGL_ENABLED
#include <drivers/opengl/shader_loader_glsl.hpp>
#endif

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_from_file(Shader & shader, Path const & path)
{
	if (!path) { return Error_Unknown; }
	if (shader.m_shader) { RENDERING_DEVICE->shader_destroy(shader.m_shader); }

	std::ifstream file{ path.c_str() };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return Error_Unknown; }

	JSON json;
	try { json = JSON::parse(file); }
	catch (JSON::parse_error) {}
	if (!json.empty()) { return load_from_json(shader, json); }

	return Error_Unknown;
}

Error_ ShaderLoader::load_from_file(Ref<Shader> shader, Path const & path)
{
	if (!shader) { return Error_Unknown; }
	return load_from_file(**shader, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_from_json(Shader & shader, JSON const & json)
{
	JSON::const_iterator lang{ json.find("lang") };
	if ((lang == json.end()) || !lang->is_string()) { return Error_Unknown; }

	Error_ err{ Error_Unknown };
	RD::ShaderStageData spec[RD::ShaderStage_MAX] {};
	switch (hash(util::to_lower(lang->get<String>()))) {
	case "glsl"_hash: { err = generate_glsl(spec, json); } break;
	case "hlsl"_hash: { err = generate_hlsl(spec, json); } break;
	}
	if (err != Error_None) { return err; }

	shader.m_shader = RENDERING_DEVICE->shader_create(spec);
	if (!shader.m_shader) { return Error_Unknown; }
	return Error_None;
}

Error_ ShaderLoader::load_from_json(Ref<Shader> shader, JSON const & json)
{
	if (!shader) { return Error_Unknown; }
	return load_from_json(**shader, json);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::generate_glsl(RD::ShaderStageData (&spec)[RD::ShaderStage_MAX], JSON const & json)
{
	return ShaderLoaderGLSL::generate_glsl(spec, json);
}

Error_ ShaderLoader::generate_hlsl(RD::ShaderStageData (&spec)[RD::ShaderStage_MAX], JSON const & json)
{
	return Error_None;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */