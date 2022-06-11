#if OPENGL_ENABLED

#include <drivers/opengl/shader_loader_glsl.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static constexpr cstring _stage_names[RD::ShaderStage_MAX]
{
	"vertex",
	"pixel",
	"geometry",
	"tess_eval",
	"tess_ctrl",
	"compute",
};

void process_shader_stage(RD::ShaderStage_ shader_stage, RD::ShaderStageData(&spec)[RD::ShaderStage_MAX], JSON const & json)
{
	JSON::const_iterator stage{ json.find(_stage_names[shader_stage]) };
	if (stage == json.end()) { return; }
	spec[shader_stage].shader_stage = shader_stage;
	DynamicBuffer & code{ spec[shader_stage].code };
	code.print("#version 460 core\n");

	enum : size_t { IN, OUT, DATA, MAIN, MAX_SECTOR };
	constexpr cstring section_name[MAX_SECTOR]{ "in", "out", "data", "main", };
	JSON::const_pointer section[MAX_SECTOR]{};
	for (size_t i = 0; i < MAX_SECTOR; ++i) {
		if (auto const it{ stage->find(section_name[i]) }; it != stage->end()) {
			section[i] = &(*it);
		}
	}
	
	if (section[IN])
	{
		for (auto const & [k, v] : section[IN]->items())
		{
			int32_t i{ -1 };
			if (auto const it{ v.find("bind") }; it != v.end() && it->is_number()) { it->get_to(i); }
			else { continue; }

			String t{};
			if (auto const it{ v.find("type") }; it != v.end() && it->is_string()) { it->get_to(t); }
			else { continue; }

			code.printf("layout (location = %i) in %.*s %.*s;\n", i, t.size(), t.data(), k.size(), k.data());
		}
	}

	if (section[OUT])
	{
		for (auto const & [k, v] : section[OUT]->items())
		{
			int32_t i{ -1 };
			if (auto const it{ v.find("bind") }; it != v.end() && it->is_number()) { it->get_to(i); }
			else { continue; }

			String t{};
			if (auto const it{ v.find("type") }; it != v.end() && it->is_string()) { it->get_to(t); }
			else { continue; }

			code.printf("layout (location = %i) out %.*s %.*s;\n", i, t.size(), t.data(), k.size(), k.data());
		}
	}

	if (section[DATA])
	{
		for (auto const & [k, v] : section[DATA]->items())
		{
			int32_t i{ -1 };
			if (auto const it{ v.find("bind") }; it != v.end() && it->is_number()) { it->get_to(i); }
			else { continue; }

			String t{};
			if (auto const it{ v.find("type") }; it != v.end() && it->is_string()) { it->get_to(t); }
			else { continue; }

			JSON::const_pointer d{};
			if (auto const it{ v.find("data") }; it != v.end()) { d = &(*it); }

			switch (hash(t))
			{
			case "sampler2D"_hash:
			case "sampler3D"_hash:
			case "samplerCube"_hash: {
				code.printf("layout (location = %i) uniform %.*s %.*s;\n", i, t.size(), t.data(), k.size(), k.data());
			} break;

			case "ubo"_hash: {
				code.printf("layout (std140, binding = %zu) uniform %.*s {\n", i, k.size(), k.data());
				Vector<String> lines{};
				lines.resize(d->size());
				for (auto const & [uk, uv] : d->items())
				{
					int32_t ui{ -1 };
					if (auto const it{ uv.find("bind") }; it != uv.end() && it->is_number()) { it->get_to(ui); }
					else { continue; }

					String ut{};
					if (auto const it{ uv.find("type") }; it != uv.end() && it->is_string()) { it->get_to(ut); }
					else { continue; }

					ASSERT((size_t)ui < lines.size());
					lines[(size_t)ui] = "    "_s + (String)ut + " "_s + (String)uk + ";\n"_s;
				}
				for (String const & line : lines)
				{
					code.print(line);
				}
				code.print("};\n");
			} break;
			}
		}
	}
	
	code.print("void main() {\n");
	if (section[MAIN])
	{
		for (String const & s : section[MAIN]->get<Vector<String>>())
		{
			code.printf("    %.*s\n", s.size(), s.data());
		}
	}
	code.print("}\n");

	code << '\0';
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoaderGLSL::generate_glsl(RD::ShaderStageData(&spec)[RD::ShaderStage_MAX], JSON const & json)
{
	process_shader_stage(RD::ShaderStage_Vertex, spec, json);
	process_shader_stage(RD::ShaderStage_Pixel, spec, json);
	process_shader_stage(RD::ShaderStage_Geometry, spec, json);
	process_shader_stage(RD::ShaderStage_TesselationControl, spec, json);
	process_shader_stage(RD::ShaderStage_TesselationEvaluation, spec, json);
	process_shader_stage(RD::ShaderStage_Compute, spec, json);
	return Error_None;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED