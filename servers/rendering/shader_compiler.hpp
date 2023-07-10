#ifndef _ISM_SHADER_COMPILER_HPP_
#define _ISM_SHADER_COMPILER_HPP_

#include <servers/rendering/shader_language.hpp>
#include <servers/rendering_server.hpp>

namespace Ism
{
	class ISM_API ShaderCompiler
	{
		ShaderLanguage m_parser{};

	public:
		struct IdentifierActions
		{
		};

		struct GeneratedCode
		{
		};

		struct DefaultIdentifierActions
		{
		};

	public:
		ShaderCompiler();

		void initialize(DefaultIdentifierActions actions);

		Error_ compile(
			RS::ShaderMode_		mode,
			String const &		code,
			IdentifierActions *	actions,
			String const &		path,
			GeneratedCode &		r_out);
	};
}

#endif // !_ISM_SHADER_COMPILER_HPP_
