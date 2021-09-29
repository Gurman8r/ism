#include <core/object/cppfunction_object.hpp>
#include <core/object/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(CppFunctionObject, t, TypeFlags_BaseType | TypeFlags_HaveVectorCall | TypeFlags_MethodDescriptor)
{
	t.tp_dictoffset = offsetof(CppFunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(CppFunctionObject, m_vectorcall);

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ cls) -> OBJ
	{
		return !obj ? self : METHOD({ self, obj, api::method_vectorcall });
	};

	t.tp_bind = CLASS_BINDFUNC(CppFunctionObject, t)
	{
		t.add_object("__name__", PROPERTY({
			CPP_FUNCTION({ [](CppFunctionObject const & self) -> String const & { return self->name; }, api::attr::is_method(t) }),
			CPP_FUNCTION({ [](CppFunctionObject & self, String const & value) { self->name = value; }, api::attr::is_method(t) }),
			}));

		return t

			.def_property("__text_signature__", [](CppFunctionObject const & self) { return self->signature; }, [](CppFunctionObject & self, String const & value) { self->signature = value; })

			;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

CppFunctionObject::~CppFunctionObject()
{
	while (m_record)
	{
		api::FunctionRecord * next{ m_record->next };

		memdelete(m_record);

		m_record = next;
	}
}

CppFunctionObject::CppFunctionObject() noexcept : FunctionObject{ api::cppfunction_vectorcall }
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CppFunctionObject::initialize_generic(api::FunctionRecord * rec, std::type_info const * const * info_in, size_t argc_in, bool prepend)
{
	VERIFY("BAD FUNCTION RECORD" && rec && !rec->next);

	m_record = rec;

	rec->argument_count = argc_in;

	rec->args.reserve(argc_in);

	for (size_t i = 0; i < argc_in; ++i)
	{
		String const arg_str{ "arg" + util::to_string(i) };

		rec->args.push_back(api::ArgumentRecord{ arg_str, nullptr, false, false });
	}

	// overload chaining
	if (CPP_FUNCTION::check_(rec->sibling))
	{
		api::FunctionRecord *& chain{ ((CppFunctionObject *)(rec->sibling))->m_record };

		if (rec->scope == chain->scope)
		{
			if (prepend)
			{
				rec->next = chain;
				chain = nullptr;
			}
			else
			{
				api::FunctionRecord * it{ chain };
				while (it->next) { it = it->next; }
				it->next = rec;
				m_record = chain;
				chain = nullptr;
			}
		}
	}

	if (!rec->next && rec->scope)
	{
		if (STR_IDENTIFIER(__module__); api::hasattr(rec->scope, &ID___module__)) { m_module = api::getattr(rec->scope, &ID___module__); }

		else if (STR_IDENTIFIER(__name__); api::hasattr(rec->scope, &ID___name__)) { m_module = api::getattr(rec->scope, &ID___name__); }
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ ism::api::cppfunction_vectorcall(OBJ callable, OBJ const * argv, size_t argc)
{
	if (!CPP_FUNCTION::check_(callable)) { return nullptr; }

	OBJ parent{ 0 < argc ? argv[0] : nullptr };

	api::FunctionRecord const * overloads{ &***CPP_FUNCTION(callable) }, * it{ overloads };

	for (; it; it = it->next)
	{
		bool const overloaded{ it && it->next };

		api::FunctionRecord const & func{ *it };

		api::FunctionCall call{ func, parent };

		size_t
			num_args	{ func.argument_count },
			num_to_copy	{ MIN(num_args, argc) },
			num_copied	{};

		// copy positional arguments
		for (; num_copied < num_to_copy; ++num_copied)
		{
			api::ArgumentRecord const * arg{};

			if (num_copied < func.args.size()) { arg = &func.args[num_copied]; }

			call.args.push_back(argv[num_copied], arg && arg->convert);
		}

		// fill in missing arguments
		if (num_copied < num_args)
		{
			for (; num_copied < num_args; ++num_copied)
			{
				api::ArgumentRecord const & arg{ func.args[num_copied] };

				if (!arg.value) { break; }

				else { call.args.push_back(arg.value, arg.convert); }
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