#include <servers/rendering/shader_compiler.hpp>

namespace Ism
{
	ShaderCompiler::ShaderCompiler()
	{
	}

	void ShaderCompiler::initialize(DefaultIdentifierActions actions)
	{
	}

	Error_ ShaderCompiler::compile(RS::ShaderMode_ mode, String const & code, IdentifierActions * actions, String const & path, GeneratedCode & r_out)
	{
		return Error_OK;
	}
}