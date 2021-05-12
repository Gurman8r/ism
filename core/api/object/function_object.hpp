#ifndef _ISM_FUNCTION_OBJECT_HPP_
#define _ISM_FUNCTION_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// function
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API CoreFunction : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreFunction, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	protected:
		vectorcallfunc m_vectorcall{};

	public:
		explicit CoreFunction(TYPE const & t, vectorcallfunc vectorcall) : base_type{ t }, m_vectorcall{ vectorcall } {}

		explicit CoreFunction(vectorcallfunc vectorcall) : self_type{ &ob_type_static, vectorcall } {}

		NODISCARD vectorcallfunc get_vectorcall() const { return m_vectorcall; }

		void set_vectorcall(vectorcallfunc vectorcall) { m_vectorcall = vectorcall; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FUNCTION_OBJECT_HPP_
