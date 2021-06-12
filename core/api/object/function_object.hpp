#ifndef _ISM_FUNCTION_OBJECT_HPP_
#define _ISM_FUNCTION_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// function object
class ISM_API ism::api::FunctionObject : public BaseObject
{
	ISM_OBJECT(FunctionObject, BaseObject);

protected:
	static void _bind_methods(TypeObject & t);

public:
	vectorcallfunc m_vectorcall{};

public:
	explicit FunctionObject(TypeObject const * t, vectorcallfunc vectorcall) : base_type{ t }, m_vectorcall{ vectorcall } {}

	explicit FunctionObject(vectorcallfunc vectorcall) : self_type{ &ob_type_static, vectorcall } {}

	NODISCARD vectorcallfunc get_vectorcall() const { return m_vectorcall; }

	void set_vectorcall(vectorcallfunc vectorcall) { m_vectorcall = vectorcall; }
};

// function deleter
template <> struct ism::DefaultDelete<ism::api::FunctionObject> : DefaultDelete<ism::api::BaseObject> {};

// function handle
template <> class ism::api::Handle<ism::api::FunctionObject> : public BaseHandle<FunctionObject>
{
	ISM_HANDLE(FunctionObject);

public:
	Handle() = default;

	~Handle() = default;
};

#endif // !_ISM_FUNCTION_OBJECT_HPP_
