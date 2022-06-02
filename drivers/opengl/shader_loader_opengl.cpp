#if OPENGL_ENABLED

#include <drivers/opengl/shader_loader_opengl.hpp>
#include <core/io/filesystem.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

constexpr cstring macro(cstring str, size_t size)
{
	switch (hash(str, size))
	{
	// float
	case hash("double"): case hash("float"): return "float";
	case hash("vec2d"): case hash("vec2f"): return "vec2";
	case hash("vec3d"): case hash("vec3f"): return "vec3";
	case hash("vec4d"): case hash("vec4f"): return "vec4";
	case hash("mat3d"): case hash("mat3f"): return "mat3";
	case hash("mat4d"): case hash("mat4f"): return "mat4";

	// int
	case hash("bool"): case hash("int"): return "int";
	case hash("vec2b"): case hash("vec2i"): return "ivec2";
	case hash("vec3b"): case hash("vec3i"): return "ivec3";
	case hash("vec4b"): case hash("vec4i"): return "ivec4";
	case hash("mat3b"): case hash("mat3i"): return "imat3";
	case hash("mat4b"): case hash("mat4i"): return "imat4";

	// texture
	case hash("texture2d"): return "sampler2D";
	case hash("texture3d"): return "sampler3D";
	case hash("cubemap"): return "samplerCube";
	}
	return str;
}

constexpr cstring macro(cstring value) { return macro(value, util::strlen(value)); }

inline String macro(String const & value) { return macro(value.data(), value.size()); }

struct ShaderImportData
{
	JSON const & json;
	
	Vector<std::pair<String, String>> defines{};
	
	RD::ShaderCreateInfo spec{};

	ShaderImportData(JSON const & json) : json{ json }
	{
		ASSERT(!json.empty());

		// defines
		if (auto def{ json.find("defines") }; def != json.end()) {
			for (auto & [key, value] : def->items()) {
				defines.push_back({ (String)key, value.is_string() ? value.get<String>() : (String)value.dump() });
			}
		}

		// stages
		process_stage("vertex", RD::ShaderStage_Vertex);
		process_stage("pixel", RD::ShaderStage_Fragment);
		process_stage("geometry", RD::ShaderStage_Geometry);
		process_stage("tess_ctrl", RD::ShaderStage_TesselationControl);
		process_stage("tess_eval", RD::ShaderStage_TesselationEvaluation);
		process_stage("compute", RD::ShaderStage_Compute);
		for (RD::ShaderStageData stage_data : spec.stage_data) {
			ASSERT(stage_data.shader_stage != RD::ShaderStage_MAX);
		}
	}

	Error_ process_stage(cstring stage_name, RD::ShaderStage_ shader_stage)
	{
		RD::ShaderStageData & stage_data{ spec.stage_data[shader_stage] };
		ASSERT(stage_data.shader_stage == shader_stage);
		DynamicBuffer & code{ stage_data.code };
		auto stage{ json.find(stage_name) };
		if (stage == json.end()) { return Error_Unknown; }

		// version
		static String const version_str{ "#version 460 core\n"_s };
		code.write(version_str.data(), version_str.size());

		// defines
		if (!defines.empty()) {
			for (auto & [key, value] : defines) {
				String const line{ "#define "_s + key + " "_s + value + "\n"_s };
				code.write(line.data(), line.size());
			}
		}

		// inputs
		if (auto in{ stage->find("in") }; in != stage->end() && !in->empty())
		{
			if (shader_stage == RD::ShaderStage_Vertex)
			{
				for (size_t i = 0; i < in->size(); ++i) {
					String const name{ (*in)[i]["name"] };
					String const type{ macro((*in)[i]["type"]) };
					code.printf("layout (location = %i) in %.*s %.*s;\n",
						(int32_t)i,
						type.size(), type.data(),
						name.size(), name.data());
				}
			}
			else
			{
				for (size_t i = 0; i < in->size(); ++i) {
					String const name{ (*in)[i]["name"] };
					String const type{ macro((*in)[i]["type"]) };
					code.printf("in %.*s %.*s;\n",
						type.size(), type.data(),
						name.size(), name.data());
				}
			}
		}

		// outputs
		if (auto out{ stage->find("out") }; out != stage->end() && !out->empty())
		{
			for (size_t i = 0; i < out->size(); ++i) {
				String const name{ (*out)[i]["name"] };
				String const type{ macro((*out)[i]["type"]) };
				code.printf("out %.*s %.*s;\n",
					type.size(), type.data(),
					name.size(), name.data());
			}
		}

		// buffers
		if (auto buffers{ stage->find("buffers") }; buffers != stage->end())
		{
			for (size_t i = 0; i < buffers->size(); ++i)
			{
				String const name{ (*buffers)[i]["name"] };
				String const type{ macro((*buffers)[i]["type"]) };
				switch (hash(type)) {

				// textures
				case hash("sampler2D"):
				case hash("sampler3D"):
				case hash("samplerCube"): {
					code.printf("layout (location = %i) uniform %.*s %.*s;\n",
						(int32_t)i,
						type.size(), type.data(),
						name.size(), name.data());
				} break;

				// uniform buffer
				case hash("uniform_buffer"): {
					code.printf("layout (std140, binding = %i) uniform %.*s {\n",
						(int32_t)i,
						name.size(), name.data());
					if (auto subdata{ (*buffers)[i].find("data") }; subdata != (*buffers)[i].end()) {
						for (size_t j = 0; j < subdata->size(); ++j) {
							String const subdata_name{ (*subdata)[j]["name"] };
							String const subdata_type{ macro((*subdata)[j]["type"]) };
							code.printf("%.*s %.*s;\n",
								subdata_type.size(), subdata_type.data(),
								subdata_name.size(), subdata_name.data());
						}
					}
					code.print("};\n");
				} break;

				}
			}
		}

		// main
		if (auto main{ stage->find("main") }; main != stage->end())
		{
			code.print("void main() {\n");
			for (String const & line : main->get<Vector<String>>()) {
				code.printf("%.*s\n", line.size(), line.data());
			}
			code.print("}\n");
		}

		code << '\0';

		OS->print(String{ code.c_str(), code.size() });

		return Error_None;
	}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RD::ShaderCreateInfo ShaderLoaderOpenGL::create_shader_spec(JSON const & json)
{
	return ShaderImportData{ json }.spec;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED