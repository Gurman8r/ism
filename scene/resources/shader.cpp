#include <scene/resources/shader.hpp>
#include <servers/rendering_server.hpp>
#include <core/io/filesystem.hpp>

using namespace ism;

OBJECT_EMBED(Shader, t) {}

Shader::~Shader()
{
	if (m_shader) { RENDERING_DEVICE->shader_destroy(m_shader); }
}

void Shader::reload_from_file()
{
	if (get_path().empty()) { return; }

	if (m_shader) { RENDERING_DEVICE->shader_destroy(m_shader); }

	std::ifstream file{ get_path().c_str() };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return; }
	
	JSON json{ JSON::parse(file) };
	if (json.empty()) { return; }

	Vector<RD::ShaderStageData> stages{};

	auto _parse_stage = [&json, &stages
	](cstring stage_name, ShaderStage_ stage_index)
	{
		RD::ShaderStageData data{ stage_index };

		if (auto stage{ json.find(stage_name) }; stage != json.end())
		{
			// GLSL
			if (auto glsl{ stage->find("glsl") }; glsl != stage->end())
			{
				for (String const & line : glsl->get<Vector<String>>()) {
					data.source_code.write(line.data(), line.size());
					data.source_code << '\n';
				}
				data.source_code << '\0';
			}
			// PATH
			else if (auto path{ stage->find("path") }; path != stage->end())
			{
				std::ifstream file{ path->get<Path>().c_str() };
				SCOPE_EXIT(&file) { file.close(); };
				if (!file) { return; }
				String line;
				while (std::getline(file, line)) {
					data.source_code.write(line.data(), line.size());
					data.source_code << '\n';
				}
				data.source_code << '\0';
			}
		}
		if (!data.source_code.empty())
		{
			stages.push_back(std::move(data));
		}
	};

	_parse_stage("vertex", ShaderStage_Vertex);
	_parse_stage("pixel", ShaderStage_Fragment);
	_parse_stage("geometry", ShaderStage_Geometry);
	_parse_stage("tess_ctrl", ShaderStage_TesselationControl);
	_parse_stage("tess_eval", ShaderStage_TesselationEvaluation);
	_parse_stage("compute", ShaderStage_Compute);
	m_shader = RENDERING_DEVICE->shader_create(stages);
	ASSERT(m_shader);
}

void Shader::bind()
{
	RENDERING_DEVICE->shader_bind(m_shader);
}

void Shader::unbind()
{
	RENDERING_DEVICE->shader_bind(nullptr);
}

void Shader::set_uniform1i(String const & name, int32_t const value)
{
	RENDERING_DEVICE->shader_uniform1i(m_shader, name, value);
}

void Shader::set_uniform1f(String const & name, float_t const value)
{
	RENDERING_DEVICE->shader_uniform1f(m_shader, name, value);
}

void Shader::set_uniform2f(String const & name, Vec2f const & value)
{
	RENDERING_DEVICE->shader_uniform2f(m_shader, name, value);
}

void Shader::set_uniform3f(String const & name, Vec3f const & value)
{
	RENDERING_DEVICE->shader_uniform3f(m_shader, name, value);
}

void Shader::set_uniform4f(String const & name, Vec4f const & value)
{
	RENDERING_DEVICE->shader_uniform4f(m_shader, name, value);
}

void Shader::set_uniform16f(String const & name, Mat4f const & value, bool transpose)
{
	RENDERING_DEVICE->shader_uniform16f(m_shader, name, value, transpose);
}

void Shader::set_uniform_texture(String const & name, RID const value, size_t slot)
{
	RENDERING_DEVICE->texture_bind(value, slot);

	RENDERING_DEVICE->shader_uniform1i(m_shader, name, slot);
}
