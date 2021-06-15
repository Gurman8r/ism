#ifndef _ISM_CPPFUNCTION_OBJECT_HPP_
#define _ISM_CPPFUNCTION_OBJECT_HPP_

#include <core/api/detail/initimpl.hpp>

// cppfunction object
class NODISCARD ISM_API ism::api::CppFunctionObject : public FunctionObject
{
	ISM_OBJECT_CVT(CppFunctionObject, FunctionObject);

protected:
	static void _bind_class(TypeObject & t);

	function_record m_cppfunction{};

public:
	using storage_type = decltype(m_cppfunction);

	CppFunctionObject() noexcept : base_type{ &ob_type_static, dispatcher } {}

	template <class Func, class ... Extra> CppFunctionObject(Func && f, Extra && ... extra) : self_type{ storage_type{ FWD(f), FWD(extra)... } } {}

	NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_cppfunction); }

	NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_cppfunction); }

protected:
	explicit CppFunctionObject(storage_type const & value) : self_type{} { m_cppfunction = value; }

	explicit CppFunctionObject(storage_type && value) noexcept : self_type{} { m_cppfunction = std::move(value); }

	static OBJECT dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
	{
		if (!callable) { return nullptr; }

		function_record const & func{ **super_cast<CppFunctionObject>(*callable) };
			
		function_call call{ func, (0 < argc ? argv[0] : nullptr) };
			
		return func.impl(call(argv, argc));
	}
};

// cppfunction deleter
template <> struct ism::DefaultDelete<ism::api::CppFunctionObject> : ism::DefaultDelete<ism::api::BaseObject> {};

// cppfunction handle
template <> class ism::api::Handle<ism::api::CppFunctionObject> : public ism::api::BaseHandle<ism::api::CppFunctionObject>
{
	ISM_HANDLE(CppFunctionObject);

public:
	Handle() = default;

	~Handle() = default;

	using storage_type = CppFunctionObject::storage_type;

	NODISCARD OBJECT name() const { return attr("__name__"); }
};

#endif // !_ISM_CPPFUNCTION_OBJECT_HPP_
