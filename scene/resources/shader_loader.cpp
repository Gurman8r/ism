#include <scene/resources/shader_loader.hpp>
#include <servers/rendering_server.hpp>
#include <core/io/filesystem.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct ShaderImportData
{
	std::ifstream file;

	JSON json{};

	RD::ShaderCreateInfo spec{};

	NODISCARD operator bool() const noexcept { return file.operator bool(); }

	~ShaderImportData() { if (file) { file.close(); } }

	explicit ShaderImportData(Path const & path) : file{ path.c_str() }
	{
		if (file) { json = JSON::parse(file); }
	}
};

static void _parse_text(RD::ShaderStageData & stage_data, JSON & text)
{
	for (String const & line : text.get<Vector<String>>()) {
		stage_data.code.write(line.data(), line.size());
		stage_data.code << '\n';
	}
	stage_data.code << '\0';
}

static void _parse_file(RD::ShaderStageData & stage_data, JSON & path)
{
	std::ifstream file{ path.get<Path>().c_str() };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return; }
	String line;
	while (std::getline(file, line)) {
		stage_data.code.write(line.data(), line.size());
		stage_data.code << '\n';
	}
	stage_data.code << '\0';
}

static void _process_stage(ShaderImportData & import_data, cstring stage_name, RD::ShaderStage_ shader_stage)
{
	RD::ShaderStageData & stage_data{ import_data.spec.stage_data[shader_stage] };
	ASSERT(stage_data.shader_stage == shader_stage);
	auto stage{ import_data.json.find(stage_name) };
	if (stage == import_data.json.end()) { return; }

	// TEXT
	if (auto text{ stage->find("text") }; text != stage->end())
	{
		_parse_text(stage_data, *text);
	}
	// PATH
	else if (auto path{ stage->find("path") }; path != stage->end())
	{
		_parse_file(stage_data, *path);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_shader(Shader & shader, Path const & path)
{
	if (!path) { return Error_Unknown; }

	if (shader.m_shader) { RENDERING_DEVICE->shader_destroy(shader.m_shader); }

	ShaderImportData import_data{ path };
	if (!import_data) { return Error_Unknown; }
	_process_stage(import_data, "vertex", RD::ShaderStage_Vertex);
	_process_stage(import_data, "pixel", RD::ShaderStage_Fragment);
	_process_stage(import_data, "geometry", RD::ShaderStage_Geometry);
	_process_stage(import_data, "tess_ctrl", RD::ShaderStage_TesselationControl);
	_process_stage(import_data, "tess_eval", RD::ShaderStage_TesselationEvaluation);
	_process_stage(import_data, "compute", RD::ShaderStage_Compute);
	shader.m_shader = RENDERING_DEVICE->shader_create(import_data.spec);
	ASSERT(shader.m_shader);
	
	return Error_None;
}

Error_ ShaderLoader::load_shader(Ref<Shader> shader, Path const & path)
{
	if (!shader) { return Error_Unknown; }

	return load_shader(**shader, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLoader::load_shader(Shader & shader, DynamicBuffer const & code)
{
	return Error_None;
}

Error_ ShaderLoader::load_shader(Ref<Shader> shader, DynamicBuffer const & code)
{
	if (!shader) { return Error_Unknown; }

	return load_shader(**shader, code);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */