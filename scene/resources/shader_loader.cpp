#include <scene/resources/shader_loader.hpp>
#include <servers/rendering_server.hpp>
#include <fstream>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class ImplShaderLoader
{
public:
	struct Def
	{
		String name{};
		String type{};
		int32_t index{ -1 };
		Json::const_pointer data{};

		operator bool() const noexcept { return (-1 < index) && !name.empty() && !type.empty(); }

		explicit Def(Json::string_t const & key, Json const & value)
		{
			if (key.empty() || value.empty()) { return; }
			name = (String)key;
			for (auto const & [k, v] : value.items())
			{
				switch (ism::hash(k.data(), k.size())) {
				// bool
				case "bool"_hash: case "bvec2"_hash: case "bvec3"_hash: case "bvec4"_hash:
				// int
				case "int"_hash: case "ivec2"_hash: case "ivec3"_hash: case "ivec4"_hash:
				// uint
				case "uint"_hash: case "uvec2"_hash: case "uvec3"_hash: case "uvec4"_hash:
				// float
				case "float"_hash: case "vec2"_hash: case "vec3"_hash: case "vec4"_hash:
				case "mat2"_hash: case "mat3"_hash: case "mat4"_hash:
				case "mat2x2"_hash: case "mat2x3"_hash: case "mat2x4"_hash:
				case "mat3x2"_hash: case "mat3x3"_hash: case "mat3x4"_hash:
				case "mat4x2"_hash: case "mat4x3"_hash: case "mat4x4"_hash:
				// double
				case "double"_hash: case "dvec2"_hash: case "dvec3"_hash: case "dvec4"_hash:
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
				case "isampler2DMS"_hash: case "isampler2DMSArray"_hash:
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
				case "ubo"_hash:
				// default
				{
					type = k;
					if (v.is_number()) { v.get_to(index); }
					else if (v.is_string())
					{
						switch (String const s{ v }; ism::hash(s.data(), s.size())) {
						default:				{ index = -1; } break;
						case "POSITION"_hash:	{ index = 0; } break;
						case "NORMAL"_hash:		{ index = 1; } break;
						case "TEXCOORD"_hash:	{ index = 2; } break;
						case "TANGENT"_hash:	{ index = 3; } break;
						case "BITANGENT"_hash:	{ index = 4; } break;
						}
					}
				} break;

				// data
				case "data"_hash: { data = std::addressof(v); } break;
				}
			}
		}
	};

	RD::ShaderStageData spec[RD::ShaderStage_MAX]{};

	List<Pair<String, String>> global_defines{};

	ImplShaderLoader(Json const & json)
	{
		if (json.empty()) { return; }

		// global defines
		if (auto const it{ json.find("defines") }; it != json.end() && it->is_array())
		{
			for (String line : it->get<List<String>>())
			{
				if (line.trim().empty()) { continue; }
				if (size_t const i{ line.find_first_of('=') }
				; i == line.npos) {
					global_defines.push_back({ line, String{} });
				}
				else {
					String first{ line.substr(0, i) }, second{ line.substr(i + 1, line.size() - i) };
					if (first.trim().empty() || second.trim().empty()) { continue; }
					global_defines.push_back({ first, second });
				}
			}
		}

		// load stages
		for (size_t i = 0; i < RD::ShaderStage_MAX; ++i)
		{
			RD::ShaderStage_ const shader_stage{ (RD::ShaderStage_)i };
			constexpr cstring stage_name[RD::ShaderStage_MAX]{ "vertex", "fragment", "geometry", "tess_eval", "tess_ctrl", "compute", };
			Json::const_iterator const stage{ json.find(stage_name[shader_stage]) };
			if (stage == json.end()) { spec[shader_stage].shader_stage = RD::ShaderStage_MAX; return; }
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

			// global defines
			for (auto const & [first, second] : global_defines) {
				if (second.empty()) { code.printf("#define %.*s", first.size(), first.data()); }
				else { code.printf("#define %.*s %.*s\n", first.size(), first.data(), second.size(), second.data()); }
			}

			// local defines
			if (section[DEFINES] && section[DEFINES]->is_array())
			{
				for (String line : section[DEFINES]->get<List<String>>())
				{
					if (line.trim().empty()) { continue; }
					if (size_t const i{ line.find_first_of('=') }
					; i == line.npos) {
						code.printf("#define %.*s", line.size(), line.data());
					}
					else {
						String first{ line.substr(0, i) }, second{ line.substr(i + 1, line.size() - i) };
						if (first.trim().empty() || second.trim().empty()) { continue; }
						code.printf("#define %.*s %.*s\n", first.size(), first.data(), second.size(), second.data());
					}
				}
			}
	
			// in
			if (section[IN] && section[IN]->is_object())
			{
				List<DynamicBuffer> order; order.resize(section[IN]->size());
				for (auto const & [key, value] : section[IN]->items())
				{
					Def const def{ key, value };
					if (!def) { continue; }
					ASSERT((size_t)def.index < order.size());
					order[(size_t)def.index].printf("layout (location = %i) in %.*s %.*s;\n", def.index, def.type.size(), def.type.data(), def.name.size(), def.name.data());
				}
				for (auto const & line : order) { code.print(line); }
			}

			// out
			if (section[OUT] && section[OUT]->is_object())
			{
				List<DynamicBuffer> order; order.resize(section[OUT]->size());
				for (auto const & [key, value] : section[OUT]->items())
				{
					Def const def{ key, value };
					if (!def) { continue; }
					ASSERT((size_t)def.index < order.size());
					order[(size_t)def.index].printf("layout (location = %i) out %.*s %.*s;\n", def.index, def.type.size(), def.type.data(), def.name.size(), def.name.data());
				}
				for (auto const & line : order) { code.print(line); }
			}

			// data
			if (section[DATA] && section[DATA]->is_object())
			{
				for (auto const & [key, value] : section[DATA]->items())
				{
					Def const def{ key, value };
					if (!def) { continue; }
					switch (ism::hash(def.type.data(), def.type.size()))
					{
					// DEFAULT
					default: {
						code.printf("layout (location = %i) uniform %.*s %.*s;\n", def.index, def.type.size(), def.type.data(), def.name.size(), def.name.data());
					} break;

					// UNIFORM BUFFER
					case "ubo"_hash: {
						if (!def.data) { continue; }
						List<DynamicBuffer> uorder; uorder.resize(def.data->size());
						for (auto const & [ukey, uvalue] : def.data->items())
						{
							Def const udef{ ukey, uvalue };
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
	
			// main
			if (section[MAIN] && section[MAIN]->is_array())
			{
				code.print("void main() {\n");
				for (String const & line : section[MAIN]->get<List<String>>()) {
					code.printf("   %.*s\n", line.size(), line.data());
				}
				code.print("}\n");
			}

			// end shader
			if (code.back() != 0) { code << 0; }
			//OS::get_singleton()->printf("%.*s\n", code.size(), code.data());
		}
	}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_from_file(Shader & shader, Path const & path)
{
	if (!path) { return Error_Unknown; }
	std::ifstream file{ path.c_str() };
	ON_SCOPE_EXIT(&) { file.close(); };
	if (!file) { return Error_Unknown; }

	switch (path.extension().hash_code())
	{
	case ".glsl"_hash: {} break;
	case ".hlsl"_hash: {} break;
	case ".json"_hash: {
		Json json;
		try { json = Json::parse(file); }
		catch (Json::parse_error) { return Error_Unknown; }
		return load_from_json(shader, json);
	} break;
	case ".shader"_hash: {} break;
	}

	return Error_Unknown;
}

Error_ ShaderLoader::load_from_file(Ref<Shader> shader, Path const & path)
{
	if (!shader) { return Error_Unknown; }
	return load_from_file(**shader, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_from_json(Shader & shader, Json const & json)
{
	ImplShaderLoader builder{ json };
	if (shader.m_shader) { RD::get_singleton()->shader_destroy(shader.m_shader); }
	shader.m_shader = RD::get_singleton()->shader_create(builder.spec);
	if (!shader.m_shader) { return Error_Unknown; }
	return Error_None;
}

Error_ ShaderLoader::load_from_json(Ref<Shader> shader, Json const & json)
{
	if (!shader) { return Error_Unknown; }
	return load_from_json(**shader, json);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */