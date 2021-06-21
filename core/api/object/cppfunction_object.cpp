#include <core/api/object/cppfunction_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISM_OBJECT_TYPE_STATIC(CppFunctionObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor;

	t.tp_base = typeof<FUNCTION>();

	t.tp_dictoffset = offsetof(CppFunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(CppFunctionObject, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT o, OBJECT v) { return util::compare(*o, *v); };

	t.tp_descr_get = (descrgetfunc)[](OBJECT self, OBJECT obj, OBJECT type)->OBJECT
	{
		return !obj ? self : METHOD({ self, obj, method_vectorcall });
	};
};

void CppFunctionObject::_bind_class(OBJECT scope)
{
	CLASS_<CPP_FUNCTION>(scope, "cpp_function", get_type_static())

		//.def(init<>())

		.def_property("__name__", [](CPP_FUNCTION self) { return self->m_record.name; }, [](CPP_FUNCTION self, STR value) { self->m_record.name = value; })

		.def_property("__doc__", [](CPP_FUNCTION self) { return self->m_record.doc; }, [](CPP_FUNCTION self, STR value) { self->m_record.doc = value; })

		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CppFunctionObject::initialize_generic(std::type_info const * const * info_in, size_t argc_in)
{
	m_record.argument_count = argc_in;

	m_record.args.reserve(argc_in);

	for (size_t i = 0; i < argc_in; ++i)
	{
		// generate argument info

		String const arg_str{ "arg" + util::to_string(i) };

		m_record.args.push_back({ arg_str, nullptr, false, false });
	}

	function_record * chain{}, * chain_start{ &m_record };

	if (CPP_FUNCTION::check_(m_record.sibling))
	{
		chain = &((self_type *)m_record.sibling)->m_record;

		if (chain->scope != m_record.scope) { chain = nullptr; }
	}

	if (!chain)
	{
		OBJECT scope_module{};
		if (m_record.scope) {
			if (hasattr(m_record.scope, "__module__")) { scope_module = m_record.scope->attr("__module__"); }
			else if (hasattr(m_record.scope, "__name__")) { scope_module = m_record.scope->attr("__name__"); }
		}

		//m_ptr = PyCFunction_NewEx(m_record.def, rec_capsule.ptr(), scope_module.ptr());
	}
	else
	{
		//m_ptr = m_record.sibling.ptr();

		VERIFY(chain->is_method == m_record.is_method);

		if (m_record.prepend)
		{
			chain_start = &m_record;
			m_record.next = chain;
		}
		else
		{
			chain_start = chain;
			while (chain->next) { chain = chain->next; }
			chain->next = &m_record;
		}
	}

	//if (m_record.is_method) { m_ptr = PYBIND11_INSTANCE_METHOD_NEW(m_ptr, m_record.scope.ptr()); }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT CppFunctionObject::dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
{
	if (!CPP_FUNCTION::check_(callable)) { return nullptr; }

	OBJECT parent{ 0 < argc ? argv[0] : nullptr };

	function_record const * overloads{ &CPP_FUNCTION(callable)->m_record }, * it{ overloads };

	bool const overloaded{ it && it->next };

	for (; it; it = it->next)
	{
		function_record const & func{ *it };

		function_call call{ func, parent };

		size_t
			num_args	{ func.argument_count },
			num_to_copy	{ MIN(num_args, argc) },
			num_copied	{};

		// copy positional arguments
		for (; num_copied < num_to_copy; ++num_copied)
		{
			argument_record const * arg_rec{ num_copied < func.args.size() ? &func.args[num_copied] : nullptr };

			OBJECT arg{ argv[num_copied] };

			VERIFY("BAD ARGUMENT" && !(arg_rec && !arg_rec->none && arg.is_null()));

			call.args.push_back(arg, arg_rec ? arg_rec->convert : false);
		}

		// fill in missing arguments
		if (num_copied < num_args)
		{
			for (; num_copied < num_args; ++num_copied)
			{
				if (argument_record const & arg_rec{ func.args[num_copied] }; !arg_rec.value) { break; }

				else { call.args.push_back(arg_rec.value, arg_rec.convert); }
			}

			VERIFY("NOT ENOUGH ARGUMENTS" && (num_args <= num_copied));
		}

		// execute call
		if (OBJECT result{ func.impl(call) }; !call.try_next_overload)
		{
			return result;
		}
	}

	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */