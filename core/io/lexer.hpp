#ifndef _ISM_LEXER_HPP_
#define _ISM_LEXER_HPP_

// PLACEHOLDER

#include <core/object/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Lexer : public Object
	{
		DEFINE_CLASS(Lexer, Object);

	protected:
		Lexer() noexcept {}

	public:
		virtual ~Lexer() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_LEXER_HPP_
