#ifndef _ISM_API_TYPES_FUNCTION_OBJECT_HPP_
#define _ISM_API_TYPES_FUNCTION_OBJECT_HPP_

#include <core/api/types/type_object.hpp>

// function
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API CoreFunction : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreFunction, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	protected:
		vectorcallfunc m_vectorcall{};

	public:
		CoreFunction(TYPE const & t, vectorcallfunc vectorcall) : base_type{ t }, m_vectorcall{ vectorcall } {}

		NODISCARD vectorcallfunc get_vectorcall() const { return m_vectorcall; }

		void set_vectorcall(vectorcallfunc vectorcall) { m_vectorcall = vectorcall; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_FUNCTION_OBJECT_HPP_
