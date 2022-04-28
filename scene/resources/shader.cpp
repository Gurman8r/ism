#include <scene/resources/shader.hpp>
#include <servers/rendering_server.hpp>
#include <core/io/filesystem.hpp>

using namespace ism;

EMBED_CLASS(Shader, t) {}

Shader::~Shader()
{
	if (m_shader) { SINGLETON(RenderingDevice)->shader_destroy(m_shader); }
}

void Shader::reload_from_file()
{
	if (get_path().empty()) { return; }

	if (m_shader) { SINGLETON(RenderingDevice)->shader_destroy(m_shader); }

	std::ifstream file{ get_path().c_str() };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return; }
	
	JSON json{ JSON::parse(file) };
	if (json.empty()) { return; }

	Vector<ShaderStageData> stages{};

	auto _parse_stage = [&json, &stages
	](cstring stage_name, ShaderStage_ stage_index)
	{
		ShaderStageData data{ stage_index };

		if (auto stage{ json.find(stage_name) }; stage != json.end())
		{
			if (auto glsl{ stage->find("glsl") }; glsl != stage->end())
			{
				for (String const & line : glsl->get<Vector<String>>()) {
					data.source.write(line.data(), line.size());
					data.source << '\n';
				}
				data.source << '\0';
			}
			else if (auto path{ stage->find("path") }; path != stage->end())
			{
				std::ifstream file{ path->get<Path>().c_str() };
				SCOPE_EXIT(&file) { file.close(); };
				if (!file) { return; }

				String line;
				while (std::getline(file, line)) {
					data.source.write(line.data(), line.size());
					data.source << '\n';
				}
				data.source << '\0';
			}
		}
		if (!data.source.empty())
		{
			stages.push_back(std::move(data));
		}
	};

	_parse_stage("vertex", ShaderStage_Vertex);
	_parse_stage("fragment", ShaderStage_Fragment);
	_parse_stage("geometry", ShaderStage_Geometry);
	_parse_stage("tess_ctrl", ShaderStage_TesselationControl);
	_parse_stage("tess_eval", ShaderStage_TesselationEvaluation);
	_parse_stage("compute", ShaderStage_Compute);
	m_shader = SINGLETON(RenderingDevice)->shader_create(stages);
	ASSERT(m_shader);
}

void Shader::bind()
{
	SINGLETON(RenderingDevice)->shader_bind(m_shader);
}

void Shader::unbind()
{
	SINGLETON(RenderingDevice)->shader_bind(nullptr);
}

void Shader::set_uniform1i(cstring name, int32_t const value)
{
	SINGLETON(RenderingDevice)->shader_set_uniform1i(m_shader, name, value);
}

void Shader::set_uniform1f(cstring name, float_t const value)
{
	SINGLETON(RenderingDevice)->shader_set_uniform1f(m_shader, name, value);
}

void Shader::set_uniform2f(cstring name, Vec2f const & value)
{
	SINGLETON(RenderingDevice)->shader_set_uniform2f(m_shader, name, value);
}

void Shader::set_uniform3f(cstring name, Vec3f const & value)
{
	SINGLETON(RenderingDevice)->shader_set_uniform3f(m_shader, name, value);
}

void Shader::set_uniform4f(cstring name, Vec4f const & value)
{
	SINGLETON(RenderingDevice)->shader_set_uniform4f(m_shader, name, value);
}

void Shader::set_uniform16f(cstring name, Mat4f const & value, bool transpose)
{
	SINGLETON(RenderingDevice)->shader_set_uniform16f(m_shader, name, value, transpose);
}

void Shader::set_uniform_color(cstring name, Color const & value)
{
	set_uniform4f(name, (Vec4f)value);
}