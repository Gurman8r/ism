#include <core/api/object/cppfunction_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IMPLEMENT_CLASS_TYPE(CppFunctionObject, t)
{
	t.tp_flags = TypeFlags_Default | TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor;

	t.tp_base = typeof<FUNCTION>();

	t.tp_dictoffset = offsetof(CppFunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(CppFunctionObject, m_vectorcall);

	t.tp_compare = (cmpfunc)[](OBJECT self, OBJECT other) { return util::compare(*self, *other); };

	t.tp_descr_get = (descrgetfunc)[](OBJECT self, OBJECT obj, OBJECT type)->OBJECT
	{
		return !obj ? self : METHOD({ self, obj, method_vectorcall });
	};

	t.tp_new = (newfunc)[](TYPE type, OBJECT args) -> OBJECT
	{
		return holder_type::new_();
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

OBJECT CppFunctionObject::dispatcher(OBJECT callable, OBJECT const * argv, size_t argc)
{
	if (!CPP_FUNCTION::check_(callable)) { return nullptr; }

	OBJECT parent{ 0 < argc ? argv[0] : nullptr };

	FunctionRecord const * overloads{ &***CPP_FUNCTION(callable) }, * it{ overloads };

	for (; it; it = it->next)
	{
		bool const overloaded{ it && it->next };

		FunctionRecord const & func{ *it };

		FunctionCall call{ func, parent };

		size_t
			num_args	{ func.argument_count },
			num_to_copy	{ MIN(num_args, argc) },
			num_copied	{};

		// copy positional arguments
		for (; num_copied < num_to_copy; ++num_copied)
		{
			ArgumentRecord const * arg_rec{ num_copied < func.args.size() ? &func.args[num_copied] : nullptr };

			OBJECT arg{ argv[num_copied] };

			if (arg_rec && !arg_rec->none && arg.is_null())
			{
				FATAL("BAD ARGUMENT");
			}

			call.args.push_back(arg, arg_rec ? arg_rec->convert : false);
		}

		// fill in missing arguments
		if (num_copied < num_args)
		{
			for (; num_copied < num_args; ++num_copied)
			{
				ArgumentRecord const & arg_rec{ func.args[num_copied] };

				if (!arg_rec.value) { break; }

				else { call.args.push_back(arg_rec.value, arg_rec.convert); }
			}

			if (num_copied < num_args)
			{
				if (overloaded) { continue; }
				else
				{
					FATAL("NOT ENOUGH ARGUMENTS");
				}
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

void CppFunctionObject::initialize_generic(FunctionRecord * rec, std::type_info const * const * info_in, size_t argc_in)
{
	m_record = CHECK(rec);

	VERIFY(!rec->next);

	rec->argument_count = argc_in;

	rec->args.reserve(argc_in);

	for (size_t i = 0; i < argc_in; ++i)
	{
		String const arg_str{ "arg" + util::to_string(i) };

		rec->args.push_back(ArgumentRecord{ arg_str, nullptr, false, false });
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