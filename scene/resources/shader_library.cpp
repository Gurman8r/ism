#include <scene/resources/shader_library.hpp>
#include <servers/rendering_server.hpp>
#include <fstream>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static Error_ load_shader_default(std::ifstream & file, RD::ShaderStageData(&spec)[RD::ShaderStage_MAX])
	{
		RD::ShaderStage_ stage_index{ RD::ShaderStage_MAX };

		String line;
		while (std::getline(file, line.native()))
		{
			bool should_write{ true };
			if (line.erase_duplicates(' ').empty()) { continue; }
			else if (line.front() == '#') {
				switch (line.hash_code()) {
				case "#pragma shader vertex"_hash: { stage_index = RD::ShaderStage_Vertex; should_write = false; } break;
				case "#pragma shader pixel"_hash: { stage_index = RD::ShaderStage_Fragment; should_write = false; } break;
				case "#pragma shader geometry"_hash: { stage_index = RD::ShaderStage_Geometry; should_write = false; } break;
				case "#pragma shader tess_ctrl"_hash: { stage_index = RD::ShaderStage_TesselationControl; should_write = false; } break;
				case "#pragma shader tess_eval"_hash: { stage_index = RD::ShaderStage_TesselationEvaluation; should_write = false; } break;
				case "#pragma shader compute"_hash: { stage_index = RD::ShaderStage_Compute; should_write = false; } break;
				}
			}
			if (should_write && stage_index != RD::ShaderStage_MAX)
			{
				spec[stage_index].code.printf("%.*s\n", line.size(), line.data());
			}
		}

		for (i32 i{}; i < RD::ShaderStage_MAX; ++i) {
			spec[i].shader_stage = (RD::ShaderStage_)i;
			if (!spec[i].code.empty() && spec[i].code.back() != '\0') { spec[i].code << '\0'; }
			//get_os()->printf("%.*s\n", spec[i].code.size(), spec[i].code.data());
		}

		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ShaderFormatLoader, t) {}

	Error_ ShaderFormatLoader::load_shader(Ref<Shader> shader, String const & path)
	{
		if (!shader) { return Error_Failed; }
		if (path.empty()) { return Error_Failed; }
		std::ifstream file{ path.c_str() };
		ON_SCOPE_EXIT(&) { file.close(); };
		if (!file) { return Error_Failed; }

		switch (path.extension().hash_code())
		{
		case ".shader"_hash: {
			if (shader->m_shader) { get_gpu()->shader_destroy(shader->m_shader); }
			RD::ShaderStageData spec[RD::ShaderStage_MAX]{};
			if (auto const error{ load_shader_default(file, spec) }; error != Error_OK) { return error; }
			shader->m_shader = get_gpu()->shader_create(spec);
			if (!shader->m_shader) { return Error_Failed; }
			return Error_OK;
		} break;
		}

		return Error_Failed;
	}

	RES ShaderFormatLoader::load(String const & path, Error_ * r_error)
	{
		Ref<Shader> temp{}; temp.instance();
		if (auto const error{ load_shader(temp, path) }) { if (r_error) { *r_error = error; } temp = nullptr; }
		else if (r_error) { *r_error = Error_OK; }
		return temp;
	}

	void ShaderFormatLoader::get_recognized_extensions(Vector<String> * out) const
	{
		if (!out) { return; }
		out->push_back(".shader");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}