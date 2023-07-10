#include <scene/resources/shader_library.hpp>
#include <servers/rendering_server.hpp>
#include <core/config/project_settings.hpp>
#include <core/io/file.hpp>
#include <fstream>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ShaderBuilder
	{
	public:
		ShaderBuilder()
		{
		}
	}
	g_shader_builder{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static Error_ load_shader_default(Ref<File> & file, RD::ShaderStageData(&spec)[RD::ShaderStage_MAX])
	{
		RD::ShaderStage_ stage_index{ RD::ShaderStage_MAX };

		while (String line{ file->get_line() })
		{
			bool write{};
			auto it{ line.begin() };
			while ((it != line.end() - 1) && std::isspace(*it)) { ++it; }
			if (*it == '#') {
				String pragma{ it, line.end() };
				pragma = pragma.trim_back().erase_duplicates(' ');
				switch (pragma.hash_code()) {
				default: { write = true; } break;
				case "#pragma shader vertex"_hash: { stage_index = RD::ShaderStage_Vertex; } break;
				case "#pragma shader pixel"_hash: { stage_index = RD::ShaderStage_Fragment; } break;
				case "#pragma shader geometry"_hash: { stage_index = RD::ShaderStage_Geometry; } break;
				case "#pragma shader tess_ctrl"_hash: { stage_index = RD::ShaderStage_TesselationControl; } break;
				case "#pragma shader tess_eval"_hash: { stage_index = RD::ShaderStage_TesselationEvaluation; } break;
				case "#pragma shader compute"_hash: { stage_index = RD::ShaderStage_Compute; } break;
				}
			}
			else {
				write = true;
			}
			if (write && stage_index != RD::ShaderStage_MAX) {
				spec[stage_index].code.printf("%.*s\n", line.size(), line.data());
			}
		}

		for (i32 i{}; i < RD::ShaderStage_MAX; ++i) {
			spec[i].shader_stage = (RD::ShaderStage_)i;
			if (!spec[i].code.empty() && spec[i].code.back() != '\0') { spec[i].code << '\0'; }
			//PRINTF("%.*s\n", spec[i].code.size(), spec[i].code.data());
		}

		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(ShaderFormatLoader, t) {}

	Error_ ShaderFormatLoader::load_shader(Ref<Shader> shader, String const & path)
	{
		if (!shader) {
			return Error_Failed;
		}

		if (!File::exists(path)) {
			return Error_Failed;
		}

		Ref<File> file{ File::open(path, FileMode_Read) };
		if (!file) {
			return Error_Failed;
		}

		switch (path.extension().hash_code())
		{
		case ".shader"_hash: {
			if (shader->m_shader) { rendering_server()->shader_destroy(shader->m_shader); }
			RD::ShaderStageData spec[RD::ShaderStage_MAX]{};
			if (auto const error{ load_shader_default(file, spec) }; error != Error_OK) { return error; }
			shader->m_shader = rendering_device()->shader_create(spec);
			if (!shader->m_shader) { return Error_Failed; }
			return Error_OK;
		} break;
		}

		return Error_Failed;
	}

	RES ShaderFormatLoader::load(String const & path, Error_ * r_error)
	{
		Ref<Shader> temp{}; temp.instance();
		if (Error_ const err{ load_shader(temp, path) }) {
			if (r_error) { *r_error = err; }
			temp = nullptr;
		}
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