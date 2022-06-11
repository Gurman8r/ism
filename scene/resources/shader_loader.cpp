#include <scene/resources/shader_loader.hpp>
#include <servers/rendering_server.hpp>
#include <core/io/file_system.hpp>

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
	if (shader.m_shader) { RENDERING_DEVICE->shader_destroy(shader.m_shader); }

	// open file
	std::ifstream file{ path.c_str() };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return Error_Unknown; }

	// load json
	JSON json{ JSON::parse(file) };
	if (json.empty()) { return Error_Unknown; }

	// select language
	JSON::const_iterator lang{ json.find("lang") };
	if (lang == json.end()) { return Error_Unknown; }
	auto str_to_shader_language = [](String const & s) noexcept {
		switch (hash(util::to_lower(s))) {
		case "glsl"_hash: return RD::ShaderLanguage_GLSL;
		case "hlsl"_hash: return RD::ShaderLanguage_HLSL;
		}
		return RD::ShaderLanguage_MAX;
	};

	// load spec
	RD::ShaderCreateInfo spec;
	switch (RD::ShaderLanguage_ const shader_language{ str_to_shader_language(lang->get<String>()) })
	{
#if OPENGL_ENABLED
	case RD::ShaderLanguage_GLSL: { spec = ShaderLoaderGLSL::load_spec(json); } break;
#endif

#if DIRECTX_ENABLED
	case RD::ShaderLanguage_HLSL: { spec = ShaderLoaderHLSL::load_spec(json); } break;
#endif
	}

	// validate spec
	size_t check{};
	for (auto const & stage : spec.stage_data) { check += (stage.shader_stage != RD::ShaderStage_MAX); }
	if (!check) { return Error_Unknown; }

	// create shader
	shader.m_shader = RENDERING_DEVICE->shader_create(spec);
	if (!shader.m_shader) { return Error_Unknown; }

	return Error_None;
}

Error_ ShaderLoader::load_shader(Ref<Shader> shader, Path const & path)
{
	return shader ? load_shader(**shader, path) : Error_Unknown;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */