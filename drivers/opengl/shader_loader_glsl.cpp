#if OPENGL_ENABLED

#include <drivers/opengl/shader_loader_glsl.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

constexpr cstring macro(cstring str, size_t size)
{
	if (!str || !*str || !size)
	{
		return nullptr;
	}
	else if (size >= 3 && str[0] == '%' && str[1] == '{' && str[size - 1] == '}')
	{
		// TODO...
	}
	switch (hash(str, size))
	{
	// float
	case "double"_hash: case "float"_hash: return "float";
	case "vec2d"_hash: case "vec2f"_hash: return "vec2";
	case "vec3d"_hash: case "vec3f"_hash: return "vec3";
	case "vec4d"_hash: case "vec4f"_hash: return "vec4";
	case "mat3d"_hash: case "mat3f"_hash: return "mat3";
	case "mat4d"_hash: case "mat4f"_hash: return "mat4";

	// int
	case "bool"_hash: case "int"_hash: return "int";
	case "vec2b"_hash: case "vec2i"_hash: return "ivec2";
	case "vec3b"_hash: case "vec3i"_hash: return "ivec3";
	case "vec4b"_hash: case "vec4i"_hash: return "ivec4";
	case "mat3b"_hash: case "mat3i"_hash: return "imat3";
	case "mat4b"_hash: case "mat4i"_hash: return "imat4";

	// texture
	case "sampler2D"_hash: return "sampler2D";
	case "sampler3D"_hash: return "sampler3D";
	case "samplerCube"_hash: return "samplerCube";
	}
	// default
	return str;
}

constexpr cstring macro(cstring value) { return macro(value, util::strlen(value)); }

inline String macro(String const & value) { return macro(value.data(), value.size()); }

inline String macro(JSON const & value)
{
	if (value.empty()) { return {}; }
	
	else if (value.is_string()) { return macro((String)value); }
	
	else { return macro((String)value.dump()); }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class NODISCARD ShaderImporterGLSL final
{
	static constexpr cstring _stage_names[RD::ShaderStage_MAX]{ "vs", "ps", "gs", "tcs", "tes", "cs", };

	JSON const & m_json;

	RD::ShaderCreateInfo m_spec{};
	
	Vector<Pair<String, String>> m_defines{};

public:
	auto operator()() const noexcept -> RD::ShaderCreateInfo const & { return m_spec; }

	explicit ShaderImporterGLSL(JSON const & json) : m_json{ json }
	{
		ASSERT(!json.empty());
		preprocess();
		process(RD::ShaderStage_Vertex);
		process(RD::ShaderStage_Pixel);
		process(RD::ShaderStage_Geometry);
		process(RD::ShaderStage_TesselationControl);
		process(RD::ShaderStage_TesselationEvaluation);
		process(RD::ShaderStage_Compute);
		postprocess();
	}

	void preprocess()
	{
		// DEFINES
		if (auto d{ m_json.find("defines") }; d != m_json.end() && !d->empty() && d->is_array())
		{
			for (auto & [k, v] : d->items())
			{
				if (auto const it{ std::find_if(m_defines.begin(), m_defines.end(),
					[&, id = hash(k)](auto const & pair) { return hash(pair.first) == id; }
				) }; it == m_defines.end())
				{
					m_defines.push_back({ (String)k, (String)v.dump() });
				}
				else
				{
					it->second = (String)v.dump();
				}
			}
		}
	}

	void process(RD::ShaderStage_ shader_stage)
	{
		JSON::const_iterator stage{ m_json.find(_stage_names[shader_stage]) };
		if (stage == m_json.end()) { return; }
		m_spec.stage_data[(size_t)shader_stage].shader_stage = shader_stage;
		DynamicBuffer & code{ m_spec.stage_data[(size_t)shader_stage].code };

		// VERSION
		code.print("#version 460 core\n");

		// DEFINES
		for (auto & [k, v] : m_defines)
		{
			code.printf("#define %.*s %.*s\n", k.size(), k.data(), v.size(), v.data());
		}

		// INPUT
		if (auto it{ stage->find("input") }; it != stage->end() && !it->empty() && it->is_array())
		{
			for (size_t i = 0; i < it->size(); ++i)
			{
				auto type{ (*it)[i].find("type") }, name{ (*it)[i].find("name") };

				if ((type != (*it)[i].end() && type->is_string()) && (name != (*it)[i].end() && name->is_string()))
				{
					String const t{ macro(*type) }, n{ macro(*name) };

					if (shader_stage == RD::ShaderStage_Vertex)
					{
						code.printf("layout (location = %zu) in %.*s %.*s;\n", i, t.size(), t.data(), n.size(), n.data());
					}
					else
					{
						code.printf("in %.*s %.*s;\n", t.size(), t.data(), n.size(), n.data());
					}
				}
			}
		}

		// OUTPUT
		if (auto it{ stage->find("output") }; it != stage->end() && !it->empty() && it->is_array())
		{
			for (size_t i = 0; i < it->size(); ++i)
			{
				auto type{ (*it)[i].find("type") }, name{ (*it)[i].find("name") };

				if ((type != (*it)[i].end() && type->is_string()) && (name != (*it)[i].end() && name->is_string()))
				{
					String const t{ macro(*type) }, n{ macro(*name) };

					code.printf("out %.*s %.*s;\n", t.size(), t.data(), n.size(), n.data());
				}
			}
		}

		// DATA
		if (auto it{ stage->find("data") }; it != stage->end() && !it->empty() && it->is_array())
		{
			for (size_t i = 0; i < it->size(); ++i)
			{
				auto type{ (*it)[i].find("type") }, name{ (*it)[i].find("name") };

				if ((type != (*it)[i].end() && type->is_string()) && (name != (*it)[i].end() && name->is_string()))
				{
					String const t{ macro(*type) }, n{ macro(*name) };

					switch (hash(t))
					{
					// TEXTURES
					case "sampler2D"_hash:
					case "sampler3D"_hash:
					case "samplerCube"_hash: {
						code.printf("layout (location = %zu) uniform %.*s %.*s;\n", i, t.size(), t.data(), n.size(), n.data());
					} break;

					// UNIFORMS
					case "UBO"_hash: {
						code.printf("layout (std140, binding = %zu) uniform %.*s {\n", i, n.size(), n.data());
						if (auto udata{ (*it)[i].find("data") }; udata != (*it)[i].end())
						{
							for (size_t j = 0; j < udata->size(); ++j)
							{
								auto utype{ (*udata)[j].find("type") }, uname{ (*udata)[j].find("name") };

								if ((utype != (*udata)[j].end() && utype->is_string()) && (uname != (*udata)[j].end() && uname->is_string()))
								{
									String const ut{ macro(*utype) }, un{ macro(*uname) };

									code.printf("%.*s %.*s;\n", ut.size(), ut.data(), un.size(), un.data());
								}
							}
						}
						code.print("};\n");
					} break;
					}
				}
			}
		}

		// MAIN
		if (auto main{ stage->find("main") }; main != stage->end() && !main->empty() && main->is_array())
		{
			code.print("void main() {\n");

			for (String const & line : main->get<Vector<String>>())
			{
				code.printf("%.*s\n", line.size(), line.data());
			}

			code.print("}\n");
		}

		// DONE
		code << '\n' << '\0';
		//OS->print(String{ code.c_str(), code.size() });
	}

	void postprocess()
	{
	}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RD::ShaderCreateInfo ShaderLoaderGLSL::create_shader_spec(JSON const & json)
{
	return ShaderImporterGLSL{ json }();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED