#include <scene/resources/shader_loader.hpp>
#include <servers/rendering_server.hpp>
#include <core/io/file_system.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_from_file(Shader & shader, Path const & path)
{
	if (!path) { return Error_Unknown; }
	if (shader.m_shader) { RENDERING_DEVICE->shader_destroy(shader.m_shader); }

	std::ifstream file{ path.c_str() };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return Error_Unknown; }

	Json json;
	try { json = Json::parse(file); }
	catch (Json::parse_error) { json = {}; }
	if (!json.empty()) {
		return load_from_json(shader, json);
	}

	return Error_Unknown;
}

Error_ ShaderLoader::load_from_file(Ref<Shader> shader, Path const & path)
{
	if (!shader) { return Error_Unknown; }
	return load_from_file(**shader, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct Def
{
	String name{};
	String type{};
	int32_t index{ -1 };
	Json::const_pointer data{};

	NODISCARD operator bool() const noexcept { return (-1 < index) && !name.empty() && !type.empty(); }

	explicit Def(String const & key, Json const & value)
	{
		if (key.empty() || value.empty()) { return; }
		name = key;
		for (auto const & [k, v] : value.items())
		{
			switch (hash(k)) {
			// bool
			case "bool"_hash:
			case "bvec2"_hash: case "bvec3"_hash: case "bvec4"_hash:
			// int
			case "int"_hash:
			case "ivec2"_hash: case "ivec3"_hash: case "ivec4"_hash:
			// uint
			case "uint"_hash:
			case "uvec2"_hash: case "uvec3"_hash: case "uvec4"_hash:
			// float
			case "float"_hash:
			case "vec2"_hash: case "vec3"_hash: case "vec4"_hash:
			case "mat2"_hash: case "mat3"_hash: case "mat4"_hash:
			case "mat2x2"_hash: case "mat2x3"_hash: case "mat2x4"_hash:
			case "mat3x2"_hash: case "mat3x3"_hash: case "mat3x4"_hash:
			case "mat4x2"_hash: case "mat4x3"_hash: case "mat4x4"_hash:
			// double
			case "double"_hash:
			case "dvec2"_hash: case "dvec3"_hash: case "dvec4"_hash:
			case "dmat2"_hash: case "dmat3"_hash: case "dmat4"_hash:
			case "dmat2x2"_hash: case "dmat2x3"_hash: case "dmat2x4"_hash:
			case "dmat3x2"_hash: case "dmat3x3"_hash: case "dmat3x4"_hash:
			case "dmat4x2"_hash: case "dmat4x3"_hash: case "dmat4x4"_hash:
			// sampler
			case "sampler1D"_hash: case "sampler1DArray"_hash: case "sampler1DShadow"_hash: case "sampler1DArrayShadow"_hash:
			case "sampler2D"_hash: case "sampler2DArray"_hash: case "sampler2DShadow"_hash: case "sampler2DArrayShadow"_hash:
			case "sampler2DRect"_hash: case "sampler2DRectShadow"_hash:
			case "sampler2DMS"_hash: case "sampler2DMSArray"_hash:
			case "sampler3D"_hash:
			case "samplerCube"_hash: case "samplerCubeArray"_hash: case "samplerCubeShadow"_hash: case "samplerCubeArrayShadow"_hash:
			case "samplerBuffer"_hash:
			// isampler
			case "isampler1D"_hash: case "isampler1DArray"_hash:
			case "isampler2D"_hash: case "isampler2DArray"_hash:
			case "isampler2DRect"_hash:
			case "isampler2DMS"_hash:case "isampler2DMSArray"_hash:
			case "isampler3D"_hash:
			case "isamplerCube"_hash: case "isamplerCubeArray"_hash:
			case "isamplerBuffer"_hash:
			// usampler
			case "usampler1D"_hash: case "usampler1DArray"_hash:
			case "usampler2D"_hash: case "usampler2DArray"_hash:
			case "usampler2DRect"_hash:
			case "usampler2DMS"_hash: case "usampler2DMSArray"_hash:
			case "usampler3D"_hash:
			case "usamplerCube"_hash: case "usamplerCubeArray"_hash:
			case "usamplerBuffer"_hash:
			// ubo
			case "ubo"_hash: {
				type = (String)k; v.get_to(index);
			} break;

			// data
			case "data"_hash: { data = std::addressof(v); } break;
			}
		}
	}
};

static Error_ load_stage_from_json(RD::ShaderStage_ shader_stage, RD::ShaderStageData(&spec)[RD::ShaderStage_MAX], Json const & json)
{
	if (json.empty()) { return Error_Unknown; }

	// load stage
	constexpr cstring stage_name[RD::ShaderStage_MAX]{ "vertex", "fragment", "geometry", "tess_eval", "tess_ctrl", "compute", };
	Json::const_iterator const stage{ json.find(stage_name[shader_stage]) };
	if (stage == json.end()) { spec[shader_stage].shader_stage = RD::ShaderStage_MAX; return Error_None; }
	spec[shader_stage].shader_stage = shader_stage;

	// load sections
	enum : size_t { DEFINES, IN, OUT, DATA, MAIN, MAX_SECTION };
	constexpr cstring section_name[MAX_SECTION]{ "defines", "in", "out", "data", "main", };
	Json::const_pointer section[MAX_SECTION]{};
	for (size_t i = 0; i < MAX_SECTION; ++i) {
		if (auto const it{ stage->find(section_name[i]) }
		; it != stage->end()) { section[i] = &(*it); }
	}

	// begin shader
	DynamicBuffer & code{ spec[shader_stage].code };
	code.clear().print("#version 460 core\n");
	
	// IN
	if (section[IN] && section[IN]->is_object())
	{
		Vector<DynamicBuffer> order{};
		order.resize(section[IN]->size());
		for (auto const & [key, value] : section[IN]->items()) {
			Def const def{ (String)key, value };
			if (!def) { continue; }
			ASSERT((size_t)def.index < order.size());
			order[(size_t)def.index].printf("layout (location = %i) in %.*s %.*s;\n", def.index, def.type.size(), def.type.data(), def.name.size(), def.name.data());
		}
		for (auto const & line : order) { code.print(line); }
	}

	// OUT
	if (section[OUT] && section[OUT]->is_object())
	{
		Vector<DynamicBuffer> order{};
		order.resize(section[OUT]->size());
		for (auto const & [key, value] : section[OUT]->items()) {
			Def const def{ (String)key, value };
			if (!def) { continue; }
			ASSERT((size_t)def.index < order.size());
			order[(size_t)def.index].printf("layout (location = %i) out %.*s %.*s;\n", def.index, def.type.size(), def.type.data(), def.name.size(), def.name.data());
		}
		for (auto const & line : order) { code.print(line); }
	}

	// DATA
	if (section[DATA] && section[DATA]->is_object())
	{
		for (auto const & [key, value] : section[DATA]->items())
		{
			Def const def{ (String)key, value };
			if (!def) { continue; }
			switch (hash(def.type))
			{
			// UNIFORM
			default: {
				code.printf("layout (location = %i) uniform %.*s %.*s;\n", def.index, def.type.size(), def.type.data(), def.name.size(), def.name.data());
			} break;

			// UNIFORM BUFFER
			case "ubo"_hash: {
				if (!def.data) { continue; }
				Vector<DynamicBuffer> uorder{};
				uorder.resize(def.data->size());
				for (auto const & [ukey, uvalue] : def.data->items()) {
					Def const udef{ (String)ukey, uvalue };
					if (!udef) { continue; }
					ASSERT((size_t)udef.index < uorder.size());
					uorder[(size_t)udef.index].printf("   %.*s %.*s;\n", udef.type.size(), udef.type.data(), udef.name.size(), udef.name.data());
				}
				code.printf("layout (std140, binding = %i) uniform %.*s {\n", def.index, def.name.size(), def.name.data());
				for (auto const & uline : uorder) { code.print(uline); }
				code.print("};\n");
			} break;
			}
		}
	}
	
	// MAIN
	if (section[MAIN] && section[MAIN]->is_array())
	{
		code.print("void main() {\n");
		for (auto const & line : section[MAIN]->get<Vector<String>>()) {
			code.printf("   %.*s\n", line.size(), line.data());
		}
		code.print("}\n");
	}

	// end shader
	if (code.back() != byte::null) { code << byte::null; }
	SYSTEM->print("%.*s\n", code.size(), code.data());
	return Error_None;
}

static Error_ load_spec_from_json(RD::ShaderStageData(&spec)[RD::ShaderStage_MAX], Json const & json)
{
	if (json.empty()) { return Error_Unknown; }

	for (size_t i = 0; i < RD::ShaderStage_MAX; ++i)
	{
		if (Error_ const err{ load_stage_from_json((RD::ShaderStage_)i, spec, json) }
		; err != Error_None) { return err; }
	}

	return Error_None;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_from_json(Shader & shader, Json const & json)
{
	RD::ShaderStageData spec[RD::ShaderStage_MAX]{};
	if (Error_ const err{ load_spec_from_json(spec, json) }
	; err != Error_None) { return err; }

	// create shader
	shader.m_shader = RENDERING_DEVICE->shader_create(spec);
	if (!shader.m_shader) { return Error_Unknown; }
	return Error_None;
}

Error_ ShaderLoader::load_from_json(Ref<Shader> shader, Json const & json)
{
	if (!shader) { return Error_Unknown; }
	return load_from_json(**shader, json);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */