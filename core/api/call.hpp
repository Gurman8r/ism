#ifndef _ISM_CALL_HPP_
#define _ISM_CALL_HPP_

#include <core/api/cast.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument collector
	template <ReturnPolicy policy = ReturnPolicy_AutomaticReference
	> struct NODISCARD argument_collector final
	{
		template <class ... Args
		> explicit argument_collector(Args && ... values)
		{
			m_args.reserve(sizeof...(Args));

			mpl::for_args([&](auto && e) noexcept
			{
				m_args.append(make_caster<decltype(e)>::cast(FWD(e), policy, nullptr));
			}
			, FWD(values)...);
		}

		NODISCARD LIST const & args() const & { return m_args; }

		NODISCARD LIST args() && { return std::move(m_args); }

		NODISCARD OBJECT call(OBJECT callable)
		{
			if (TYPE type; !m_args || !callable || !(type = typeof(callable)))
			{
				return nullptr;
			}
			else if (vectorcallfunc vcall{ get_vectorcall_func(type, callable) })
			{
				return vcall(callable, m_args.data(), m_args.size());
			}
			else if (binaryfunc tcall{ type->tp_call })
			{
				return tcall(callable, m_args);
			}
			else
			{
				return nullptr;
			}
		}

	private:
		LIST m_args{ ListObject{} };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// collect arguments
	template <ReturnPolicy policy = ReturnPolicy_AutomaticReference, class ... Args
	> NODISCARD auto collect_arguments(Args && ... args)
	{
		return argument_collector<policy>{ FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument loader
	template <class ... Args
	> struct NODISCARD argument_loader final
	{
		template <class Return, class Guard, class Func
		> using return_type = std::conditional_t<std::is_void_v<Return>, void_type, Return>;

		static constexpr size_t argument_count{ sizeof...(Args) };

		static constexpr auto indices{ std::make_index_sequence<argument_count>() };

		std::tuple<make_caster<Args>...> argcasters{};

		NODISCARD bool load_args(Batch<OBJECT, bool> & args) { return impl_load_args(args, indices); }

		template <class Return, class Guard, class Func
		> NODISCARD auto call(Func && func) && -> return_type<Return, void_type, Return>
		{
			if constexpr (std::is_void_v<Return>)
			{
				std::move(*this).impl_call<Return>(FWD(func), indices, Guard{});

				return void_type{};
			}
			else
			{
				return std::move(*this).impl_call<Return>(FWD(func), indices, Guard{});
			}
		}

	private:
		static bool impl_load_args(Batch<OBJECT, bool> &, std::index_sequence<>) noexcept { return true; }

		template <size_t ... I> bool impl_load_args(Batch<OBJECT, bool> & args, std::index_sequence<I...>)
		{
			return !(... || !std::get<I>(argcasters).load(args.get<OBJECT>(I), args.get<bool>(I)));
		}

		template <class Return, class Func, size_t ... I, class Guard
		> Return impl_call(Func && func, std::index_sequence<I...>, Guard &&) &&
		{
			return func(cast_op<Args>(std::move(std::get<I>(argcasters)))...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument record
	struct NODISCARD argument_record final
	{
		String name{};

		OBJECT value{};

		bool convert{}, none{};

		DEFAULT_COPYABLE_MOVABLE(argument_record);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// function record
	struct NODISCARD function_record final
	{
		String name{}, doc{}, signature{};

		OBJECT(*impl)(struct function_call & call) {};

		void * data[3]{};

		void(*free_data)(function_record * record) {};

		uint16_t argument_count{};

		Vector<argument_record> args{};

		ReturnPolicy policy{ ReturnPolicy_AutomaticReference };

		bool is_stateless{}, is_constructor{}, is_operator{}, is_method{}, prepend{};

		BaseObject * scope{}, * sibling{};

		DEFAULT_COPYABLE_MOVABLE(function_record);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// function call
	struct NODISCARD function_call final
	{
		function_record const & func;
		
		OBJECT parent{};

		Batch<OBJECT, bool> args{ func.argument_count };
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class /*Derived*/, class Func
	> auto method_adaptor(Func && func) -> decltype(FWD(func)) { return FWD(func); }

	template <class Derived, class Return, class Class, class ... Args
	> auto method_adaptor(Return(Class:: * pmf)(Args...))->Return(Derived:: *)(Args...)
	{
		static_assert(
			mpl::is_accessible_base_of_v<Class, Derived>,
			"Cannot bind an inaccessible base class method; use a lambda definition instead");
		return pmf;
	}

	template <class Derived, class Return, class Class, class ... Args
	> auto method_adaptor(Return(Class:: * pmf)(Args...) const)->Return(Derived:: *)(Args...) const
	{
		static_assert(
			mpl::is_accessible_base_of_v<Class, Derived>,
			"Cannot bind an inaccessible base class method; use a lambda definition instead");
		return pmf;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> template <ReturnPolicy policy, class ...Args
	> inline OBJECT ObjectAPI<Derived>::operator()(Args && ... args) const
	{
		return ism::collect_arguments<policy>(FWD(args)...).call(handle());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CALL_HPP_
