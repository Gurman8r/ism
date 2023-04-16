#ifndef _ISM_CALL_HPP_
#define _ISM_CALL_HPP_

#include <core/object/cast.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJ call_object(OBJ callable)
	{
		if (!callable) { return nullptr; }
		else
		{
			TYPE type{ typeof(callable) };

			if (vectorcallfunc vcall{ get_vectorcall_func(type, callable) })
			{
				return vcall(callable, nullptr, 0);
			}
			else if (binaryfunc tcall{ type->tp_call })
			{
				return tcall(callable, nullptr);
			}
			else
			{
				return nullptr;
			}
		}
	}

	inline OBJ call_object(OBJ callable, LIST args)
	{
		if (!callable) { return nullptr; }
		else
		{
			TYPE type{ typeof(callable) };

			if (vectorcallfunc vcall{ get_vectorcall_func(type, callable) })
			{
				if (args)
				{
					return vcall(callable, args.data(), args.size());
				}
				else
				{
					return vcall(callable, nullptr, 0);
				}
			}
			else if (binaryfunc tcall{ type->tp_call })
			{
				return tcall(callable, args);
			}
			else
			{
				return nullptr;
			}
		}
	}

	inline OBJ call_object(OBJ callable, OBJ const * argv, size_t argc)
	{
		if (!callable) { return nullptr; }
		else
		{
			TYPE type{ typeof(callable) };

			if (vectorcallfunc vcall{ get_vectorcall_func(type, callable) })
			{
				return vcall(callable, argv, argc);
			}
			else if (binaryfunc tcall{ type->tp_call })
			{
				ListObject args{ argv, argv + argc };

				return tcall(callable, &args);
			}
			else
			{
				return nullptr;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument collector
	template <ReturnValuePolicy_ Policy = ReturnValuePolicy_AutomaticReference
	> struct ArgumentCollector final
	{
		template <class ... Args
		> explicit ArgumentCollector(Args && ... args) noexcept : m_argc{ sizeof...(Args) }
		{
			static_assert(sizeof...(Args) < MAX_ARGUMENTS, "TOO MANY ARGUMENTS");

			mpl::for_args_i([&](size_t i, auto && arg) noexcept
			{
				m_argv[i] = make_caster<decltype(arg)>::cast(FWD(arg), Policy, nullptr);
			}
			, FWD(args)...);
		}

		OBJ const * argv() const { return m_argv; }

		size_t argc() const { return m_argc; }

		OBJ call(OBJ const & callable) noexcept { return call_object(callable, m_argv, m_argc); }

	private:
		OBJ m_argv[MAX_ARGUMENTS]{};
		size_t m_argc{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument loader
	template <class ... Args
	> struct ArgumentLoader final
	{
		template <class Return, class Guard, class Func
		> using return_type = std::conditional_t<std::is_void_v<Return>, mpl::void_type, Return>;

		static constexpr size_t argument_count{ sizeof...(Args) };

		static constexpr auto indices{ std::make_index_sequence<argument_count>() };

		std::tuple<make_caster<Args>...> argcasters{};

		bool load_args(Batch<OBJ, bool> & args) { return impl_load_args(args, indices); }

		template <class Return, class Guard, class Func
		> auto call(Func && func) && -> return_type<Return, mpl::void_type, Return>
		{
			if constexpr (std::is_void_v<Return>)
			{
				std::move(*this).impl_call<Return>(FWD(func), indices, Guard{});

				return mpl::void_type{};
			}
			else
			{
				return std::move(*this).impl_call<Return>(FWD(func), indices, Guard{});
			}
		}

	private:
		static bool impl_load_args(Batch<OBJ, bool> &, std::index_sequence<>) noexcept { return true; }

		template <size_t ... I> bool impl_load_args(Batch<OBJ, bool> & args, std::index_sequence<I...>)
		{
			return !(... || !std::get<I>(argcasters).load(args.get<OBJ>(I), args.get<bool>(I)));
		}

		template <class Return, class Func, size_t ... I, class Guard
		> Return impl_call(Func && func, std::index_sequence<I...>, Guard &&) &&
		{
			return func(cast_op<Args>(std::move(std::get<I>(argcasters)))...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument record
	struct ArgumentRecord final
	{
		String name{};

		OBJ value{};

		bool convert{}, none{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// function record
	struct FunctionRecord final
	{
		~FunctionRecord() { if (free_data) { free_data(this); } }

		String name{}, signature{};

		OBJ(*impl)(struct FunctionCall &){};

		void * data[3]{};

		void(*free_data)(FunctionRecord *){};

		u16 argument_count{};

		Vector<ArgumentRecord> args{};

		ReturnValuePolicy_ policy{ ReturnValuePolicy_AutomaticReference };

		Object * scope{}, * sibling{};

		FunctionRecord * next{};

		bool is_stateless : 1, is_constructor : 1, is_operator : 1, is_method : 1;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// function call
	struct FunctionCall final
	{
		explicit FunctionCall(FunctionRecord const & record, OBJ const & parent)
			: record{ record }
			, parent{ parent }
			, args{ record.argument_count }
			, try_next_overload{}
		{
		}

		FunctionRecord const & record;
		
		OBJ parent;

		Batch<OBJ, bool> args;

		bool try_next_overload : 1;

		OBJ operator()()
		{
			OBJ result;
			{
				priv::LoaderLifeSupport guard{};
				result = record.impl(*this);
			}
			return result;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class /*Derived*/, class Func
	> auto method_adaptor(Func && func) -> decltype(FWD(func)) { return FWD(func); }

	template <class Derived, class Return, class Class, class ... Args
	> auto method_adaptor(Return(Class:: * pmf)(Args...)) -> Return(Derived:: *)(Args...)
	{
		static_assert(
			mpl::is_accessible_base_of_v<Class, Derived>,
			"Cannot bind an inaccessible base class method; use a lambda definition instead");
		return pmf;
	}

	template <class Derived, class Return, class Class, class ... Args
	> auto method_adaptor(Return(Class:: * pmf)(Args...) const) -> Return(Derived:: *)(Args...) const
	{
		static_assert(
			mpl::is_accessible_base_of_v<Class, Derived>,
			"Cannot bind an inaccessible base class method; use a lambda definition instead");
		return pmf;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object call operator
	template <class Derived
	> template <ReturnValuePolicy_ Policy, class ...Args
	> inline OBJ ObjectAPI<Derived>::operator()(Args && ... args) const
	{
		return ArgumentCollector<Policy>(FWD(args)...).call(derived().ptr());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CALL_HPP_
