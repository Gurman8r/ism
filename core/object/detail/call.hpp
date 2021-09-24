#ifndef _ISM_CALL_HPP_
#define _ISM_CALL_HPP_

#include <core/object/detail/cast.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJ call_object(OBJ callable, LIST args)
	{
		if (!callable || !args) { return nullptr; }
		else
		{
			TYPE type{ typeof(callable) };

			if (vectorcallfunc vcall{ get_vectorcall_func(type, callable) })
			{
				return vcall(callable, args.data(), args.size());
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument collector
	template <ReturnPolicy policy = ReturnPolicy_AutomaticReference
	> struct NODISCARD ArgumentCollector final
	{
		template <class ... Args
		> explicit ArgumentCollector(Args && ... values) noexcept : m_args{ LIST::new_() }
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

		NODISCARD OBJ call(OBJ const & callable) { return ism::call_object(callable, m_args); }

	private: LIST m_args;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// collect arguments
	template <ReturnPolicy policy = ReturnPolicy_AutomaticReference, class ... Args
	> NODISCARD auto collect_arguments(Args && ... args) noexcept
	{
		return ArgumentCollector<policy>{ FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> template <ReturnPolicy policy, class ...Args
	> inline OBJ ObjectAPI<Derived>::operator()(Args && ... args) const
	{
		return ism::collect_arguments<policy>(FWD(args)...).call(derived().ptr());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument loader
	template <class ... Args
	> struct NODISCARD ArgumentLoader final
	{
		template <class Return, class Guard, class Func
		> using return_type = std::conditional_t<std::is_void_v<Return>, void_type, Return>;

		static constexpr size_t argument_count{ sizeof...(Args) };

		static constexpr auto indices{ std::make_index_sequence<argument_count>() };

		std::tuple<make_caster<Args>...> argcasters{};

		NODISCARD bool load_args(Batch<OBJ, bool> & args) { return impl_load_args(args, indices); }

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
		static bool impl_load_args(Batch<OBJ, bool> &, std::index_sequence<>) noexcept { return true; }

		template <size_t ... I> bool impl_load_args(Batch<OBJ, bool> & args, std::index_sequence<I...>)
		{
			return !(... || !std::get<I>(argcasters).load(args.get<OBJ>(I), args.get<bool>(I)));
		}

		template <class Return, class Func, size_t ... I, class Guard
		> Return impl_call(Func && func, std::index_sequence<I...>, Guard &&) &&
		{
			return func(ism::cast_op<Args>(std::move(std::get<I>(argcasters)))...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument record
	struct NODISCARD ArgumentRecord final
	{
		String name{};

		OBJ value{};

		bool convert{}, none{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// function record
	struct NODISCARD FunctionRecord final
	{
		~FunctionRecord() { if (free_data) { free_data(this); } }

		String name{}, signature{};

		OBJ(*impl)(struct FunctionCall &){};

		void * data[3]{};

		void(*free_data)(FunctionRecord *){};

		uint16_t argument_count{};

		Vector<ArgumentRecord> args{};

		ReturnPolicy policy{ ReturnPolicy_AutomaticReference };

		Object * scope{}, * sibling{};

		FunctionRecord * next{};

		bool
			is_stateless	: 1,
			is_constructor	: 1,
			is_operator		: 1,
			is_method		: 1,
			prepend			: 1;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// function call
	struct NODISCARD FunctionCall final
	{
		FunctionRecord const & record;
		
		OBJ parent{};

		Batch<OBJ, bool> args{ record.argument_count };

		bool try_next_overload : 1;

		OBJ invoke() noexcept
		{
			OBJ result;
			{
				LoaderLifeSupport guard{};

				result = record.impl(*this);
			}
			return result;
		}
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
}

#endif // !_ISM_CALL_HPP_
