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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CppFunctionObject::_bind_class(OBJECT scope)
{
	CLASS_<CPP_FUNCTION>(scope, "cpp_function", get_type_static())

		//.def(init<>())

		.def_property("__name__", [](CPP_FUNCTION self) { return (***self).name; }, [](CPP_FUNCTION self, STR value) { (***self).name = value; })

		.def_property("__doc__", [](CPP_FUNCTION self) { return (***self).doc; }, [](CPP_FUNCTION self, STR value) { (***self).doc = value; })

		.def_property("__text_signature__", [](CPP_FUNCTION self) { return (***self).signature; }, [](CPP_FUNCTION self, STR value) { (***self).signature = value; })
		
		;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CppFunctionObject::~CppFunctionObject()
{
	while (m_record)
	{
		auto next{ m_record->next };

		memdelete(m_record);

		m_record = next;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CppFunctionObject::initialize_generic(function_record * rec, std::type_info const * const * info_in, size_t argc_in)
{
	VERIFY(m_record = rec);

	VERIFY(!rec->next);

	rec->argument_count = argc_in;

	rec->args.reserve(argc_in);

	for (size_t i = 0; i < argc_in; ++i)
	{
		String const arg_str{ "arg" + util::to_string(i) };

		rec->args.push_back(argument_record{ arg_str, nullptr, false, false });
	}

	if (CPP_FUNCTION::check_(rec->sibling))
	{
		if (CPP_FUNCTION sib{ rec->sibling }; rec->scope == sib->m_record->scope)
		{
			rec->next = sib->m_record;

			sib->m_record = nullptr;
		}
	}

	if (!rec->next && rec->scope)
	{
		if (hasattr(rec->scope, "__module__")) { m_module = rec->scope->attr("__module__"); }

		else if (hasattr(rec->scope, "__name__")) { m_module = rec->scope->attr("__name__"); }
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT CppFunctionObject::dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
{
	if (!CPP_FUNCTION::check_(callable)) { return nullptr; }

	OBJECT parent{ 0 < argc ? argv[0] : nullptr };

	function_record const * overloads{ &***CPP_FUNCTION(callable) }, * it{ overloads };

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

			if (arg_rec && !arg_rec->none && arg.is_null()) {
				FATAL("BAD ARGUMENT");
			}

			call.args.push_back(arg, arg_rec ? arg_rec->convert : false);
		}

		// fill in missing arguments
		if (num_copied < num_args)
		{
			for (; num_copied < num_args; ++num_copied)
			{
				argument_record const & arg_rec{ func.args[num_copied] };

				if (!arg_rec.value) { break; }

				else { call.args.push_back(arg_rec.value, arg_rec.convert); }
			}

			if (num_copied < num_args) {
				FATAL("NOT ENOUGH ARGUMENTS");
			}
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