#include <scene/resources/shader.hpp>
#include <servers/rendering_server.hpp>
#include <core/io/filesystem.hpp>

using namespace ism;

OBJECT_EMBED(Shader, t) {}

Shader::~Shader() { if (m_shader) { RENDERING_DEVICE->shader_destroy(m_shader); m_shader = nullptr; } }

struct ShaderImportData
{
	std::ifstream file;

	JSON json{};

	Vector<RD::ShaderStageData> stages{};

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
		stage_data.source_code.write(line.data(), line.size());
		stage_data.source_code << '\n';
	}
	stage_data.source_code << '\0';
}

static void _parse_file(RD::ShaderStageData & stage_data, JSON & path)
{
	std::ifstream file{ path.get<Path>().c_str() };
	SCOPE_EXIT(&file) { file.close(); };
	if (!file) { return; }
	String line;
	while (std::getline(file, line)) {
		stage_data.source_code.write(line.data(), line.size());
		stage_data.source_code << '\n';
	}
	stage_data.source_code << '\0';
}

static void _process_stage(ShaderImportData & import_data, cstring stage_name, RD::ShaderStage_ stage_index)
{
	RD::ShaderStageData stage_data{ stage_index };
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

	if (!stage_data.source_code.empty())
	{
		import_data.stages.push_back(std::move(stage_data));
	}
}

void Shader::reload_from_file()
{
	if (get_path().empty()) { return; }

	if (m_shader) { RENDERING_DEVICE->shader_destroy(m_shader); }

	ShaderImportData import_data{ get_path() };
	if (!import_data) { return; }
	_process_stage(import_data, "vertex", RD::ShaderStage_Vertex);
	_process_stage(import_data, "pixel", RD::ShaderStage_Fragment);
	_process_stage(import_data, "geometry", RD::ShaderStage_Geometry);
	_process_stage(import_data, "tess_ctrl", RD::ShaderStage_TesselationControl);
	_process_stage(import_data, "tess_eval", RD::ShaderStage_TesselationEvaluation);
	_process_stage(import_data, "compute", RD::ShaderStage_Compute);
	m_shader = RENDERING_DEVICE->shader_create(import_data.stages);
	ASSERT(m_shader);
}
