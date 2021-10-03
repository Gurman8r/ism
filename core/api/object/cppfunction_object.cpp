#include <core/api/object/cppfunction_object.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBEDDED_CLASS_TYPE(CppFunctionObject, t)
{
	t.tp_dictoffset = offsetof(CppFunctionObject, m_dict);

	t.tp_vectorcalloffset = offsetof(CppFunctionObject, m_vectorcall);

	t.tp_descr_get = (descrgetfunc)[](OBJ self, OBJ obj, OBJ cls) -> OBJ
	{
		return !obj ? self : METHOD({ self, obj });
	};

	t.tp_bind = CLASS_BINDER(CppFunctionObject, t)
	{
		// this needs to be added manually before anything else because CLASS_ depends on it to work properly
		t.add_object("__name__", PROPERTY({
			CPP_FUNCTION({ [](CppFunctionObject const & self) -> String const & { return self->name; }, attr::is_method(t) }),
			CPP_FUNCTION({ [](CppFunctionObject & self, String const & value) { self->name = value; }, attr::is_method(t) }),
			}));

		return t

			.def_property("__text_signature__",
				[](CppFunctionObject const & self) { return self->signature; },
				[](CppFunctionObject & self, String const & value) { self->signature = value; })

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void CppFunctionObject::initialize_generic(FunctionRecord * rec, std::type_info const * const * info_in, size_t argc_in, bool prepend)
{
	VERIFY("BAD FUNCTION RECORD" && rec && !rec->next);

	m_record = rec;

	rec->argument_count = argc_in;

	rec->args.reserve(argc_in);

	for (size_t i = 0; i < argc_in; ++i)
	{
		// TODO: generate argument info

		String const arg_str{ "arg" + util::to_string(i) };

		rec->args.push_back(ArgumentRecord{ arg_str, nullptr, false, false });
	}

	// overload chaining
	if (CPP_FUNCTION::check_(rec->sibling))
	{
		FunctionRecord *& existing{ ((CppFunctionObject *)(rec->sibling))->m_record };

		if (rec->scope == existing->scope)
		{
			if (prepend)
			{
				rec->next = existing;
				existing = nullptr;
			}
			else
			{
				FunctionRecord * it{ existing };
				while (it->next) { it = it->next; }
				it->next = rec;
				m_record = existing;
				existing = nullptr;
			}
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJ CppFunctionObject::cppfunction_vectorcall(OBJ callable, OBJ const * argv, size_t argc)
{
	VERIFY(argc < MAX_ARGUMENTS);

	if (!CPP_FUNCTION::check_(callable)) { return nullptr; }

	OBJ parent{ 0 < argc ? argv[0] : nullptr };

	FunctionRecord const * overloads{ &***(CPP_FUNCTION &)callable }, * it{ overloads };

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
			ArgumentRecord const * arg{};

			if (num_copied < func.args.size()) { arg = &func.args[num_copied]; }

			call.args.push_back(argv[num_copied], arg && arg->convert);
		}

		// fill in missing arguments
		if (num_copied < num_args)
		{
			for (; num_copied < num_args; ++num_copied)
			{
				ArgumentRecord const & arg{ func.args[num_copied] };

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