#ifndef _ISM_LEXER_HPP_
#define _ISM_LEXER_HPP_

// PLACEHOLDER

#include <core/object/class.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Lexer : public Object
	{
		OBJECT_CLASS(Lexer, Object);

	public:
		enum Token_ {
			Token_Name,
			Token_Int,
			Token_Float,
			Token_Char,
			Token_String,
			Token_MAX
		};

		struct Token {
			Token_ type{};
			String text{};
		};

	protected:
		Lexer() noexcept {}

	public:
		virtual ~Lexer() noexcept = default;


	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_LEXER_HPP_
