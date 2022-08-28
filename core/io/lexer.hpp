#ifndef _ISM_LEXER_HPP_
#define _ISM_LEXER_HPP_

// PLACEHOLDER

#include <core/object/detail/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Lexer : public Object
	{
		OBJECT_CLASS(Lexer, Object);

	protected:
		Lexer() noexcept {}

	public:
		virtual ~Lexer() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_LEXER_HPP_
