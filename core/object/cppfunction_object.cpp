#include <core/object/cppfunction_object.hpp>
#include <core/object/detail/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ_CLASS_IMPL(CppFunctionObject, t, "cpp_function", TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(CppFunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(CppFunctionObject, m_vectorcall);

	t.tp_new = (newfunc)[](TYPE type, OBJ args) -> OBJ { return memnew(CppFunctionObject); };

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ type)->OBJ
	{
		return !obj ? self : METHOD({ self, obj, ism::method_vectorcall });
	};

	t.tp_bind = (bindfunc)[](TYPE type) -> TYPE
	{
		DICT(type->tp_dict)["__name__"] = PROPERTY({
			CPP_FUNCTION({ [](CppFunctionObject const & self) -> String const & { return self->name; }, attr::is_method(type) }),
			CPP_FUNCTION({ [](CppFunctionObject & self, String const & value) { self->name = value; }, attr::is_method(type) }),
			});

		return CLASS_<CppFunctionObject>(type)

			.def_property("__text_signature__", [](CppFunctionObject const & self) { return self->signature; }, [](CppFunctionObject & self, String const & value) { self->signature = value; })

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CppFunctionObject::~CppFunctionObject()
{
	while (m_record)
	{
		FunctionRecord * next{ m_record->next };

		memdelete(m_record);

		m_record = next;
	}
}

CppFunctionObject::CppFunctionObject() noexcept : FunctionObject{ cppfunction_vectorcall }
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CppFunctionObject::initialize_generic(FunctionRecord * rec, std::type_info const * const * info_in, size_t argc_in)
{
	VERIFY("BAD FUNCTION RECORD" && rec && !rec->next);

	m_record = rec;

	rec->argument_count = argc_in;

	rec->args.reserve(argc_in);

	for (size_t i = 0; i < argc_in; ++i)
	{
		String const arg_str{ "arg" + util::to_string(i) };

		rec->args.push_back(ArgumentRecord{ arg_str, nullptr, false, false });
	}

	// overload chaining
	if (CPP_FUNCTION::check_(rec->sibling))
	{
		FunctionRecord *& chain{ ((CppFunctionObject *)(rec->sibling))->m_record };

		if (rec->scope == chain->scope)
		{
			if (rec->prepend)
			{
				rec->next = chain;

				chain = nullptr;
			}
			else
			{
				FunctionRecord * it{ chain };
				
				while (it->next) { it = it->next; }
				
				it->next = rec;
				
				m_record = chain;
				
				chain = nullptr;
			}
		}
	}

	if (!rec->next && rec->scope)
	{
		if (hasattr(rec->scope, "__module__")) { m_module = getattr(rec->scope, "__module__"); }

		else if (hasattr(rec->scope, "__name__")) { m_module = getattr(rec->scope, "__name__"); }
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::cppfunction_vectorcall(OBJ callable, OBJ const * argv, size_t argc)
{
	if (!CPP_FUNCTION::check_(callable)) { return nullptr; }

	OBJ parent{ 0 < argc ? argv[0] : nullptr };

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

			OBJ arg{ argv[num_copied] };

			if (arg_rec && !arg_rec->none && arg.is_null())
			{
				//FATAL("BAD NULL ARGUMENT");
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
		if (OBJ result{ call.invoke() }; !call.try_next_overload)
		{
			return result;
		}
	}

	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */