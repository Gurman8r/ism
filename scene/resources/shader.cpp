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

	auto parse_stage = [&json, &stages
	](cstring stage_name, ShaderStage_ stage_index)
	{
		ShaderStageData data{ stage_index };

		if (auto stage{ json.find(stage_name) }; stage != json.end())
		{
			if (auto glsl{ stage->find("glsl") }; glsl != stage->end())
			{
				Vector<String> lines;
				glsl->get_to(lines);
				for (String const & line : lines) {
					data.source.printf("%s\n", line.c_str());
				}
			}
			else if (auto path{ stage->find("path") }; path != stage->end())
			{
				std::ifstream file{ path->get<Path>().c_str() };
				SCOPE_EXIT(&file) { file.close(); };
				if (!file) { return; }

				String line;
				while (std::getline(file, line)) {
					data.source.printf("%s\n", line.c_str());
				}
			}
		}

		stages.push_back(data);
	};

	parse_stage("vertex", ShaderStage_Vertex);
	parse_stage("fragment", ShaderStage_Fragment);
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

void Shader::set_uniform1f(String const & name, float_t const value)
{
	SINGLETON(RenderingDevice)->shader_set_uniform1f(m_shader, name.c_str(), value);
}

void Shader::set_uniform2f(String const & name, Vec2f const & value)
{
	SINGLETON(RenderingDevice)->shader_set_uniform2f(m_shader, name.c_str(), value);
}

void Shader::set_uniform3f(String const & name, Vec3f const & value)
{
	SINGLETON(RenderingDevice)->shader_set_uniform3f(m_shader, name.c_str(), value);
}

void Shader::set_uniform4f(String const & name, Vec4f const & value)
{
	SINGLETON(RenderingDevice)->shader_set_uniform4f(m_shader, name.c_str(), value);
}

void Shader::set_uniform16f(String const & name, Mat4f const & value, bool transpose)
{
	SINGLETON(RenderingDevice)->shader_set_uniform16f(m_shader, name.c_str(), value, transpose);
}
